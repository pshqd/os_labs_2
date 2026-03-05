#pragma once

#include <cstdio>
#include <iostream>
#include "bounded_buffer.hpp"

// Структура аргументов для потока
struct ProducerArgs {
    BoundedBuffer* buffer;
    const char*    input_path;
    volatile int*  keep_running; // общий флаг для SIGINT
};

void* producer_thread(void* arg) {
    auto* args = static_cast<ProducerArgs*>(arg);

    FILE* fin = fopen(args->input_path, "rb");
    if (!fin) {
        std::cerr << "Error: cannot open input file: "
                  << args->input_path << "\n";
        // Сигнализируем consumer-у чтобы он не завис
        Block eof_block{};
        eof_block.size   = 0;
        eof_block.is_eof = true;
        args->buffer->push(eof_block);
        return nullptr;
    }

    while (*args->keep_running) {
        Block block{};
        block.size   = fread(block.data, 1, BLOCK_SIZE, fin);
        block.is_eof = false;

        if (block.size > 0) {
            args->buffer->push(block);
        }

        // Конец файла — выходим из цикла
        if (block.size < BLOCK_SIZE) {
            break;
        }
    }

    // Всегда кладём EOF-маркер, чтобы consumer завершился
    Block eof_block{};
    eof_block.size   = 0;
    eof_block.is_eof = true;
    args->buffer->push(eof_block);

    fclose(fin);
    return nullptr;
}