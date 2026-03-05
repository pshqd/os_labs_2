#pragma once

#include <cstdio>
#include <iostream>
#include "bounded_buffer.hpp"
#include "caesar.h"

struct ConsumerArgs {
    BoundedBuffer* buffer;
    const char*    output_path;
    char           key;
};

void* consumer_thread(void* arg) {
    auto* args = static_cast<ConsumerArgs*>(arg);

    FILE* fout = fopen(args->output_path, "wb");
    if (!fout) {
        std::cerr << "Error: cannot open output file: "
                  << args->output_path << "\n";
        return nullptr;
    }

    set_key(args->key);

    while (true) {
        Block block = args->buffer->pop();

        // EOF-маркер от producer — завершаем работу
        if (block.is_eof) {
            break;
        }

        // Шифруем блок на месте (src и dst совпадают — это допустимо)
        caesar(block.data, block.data, static_cast<int>(block.size));

        fwrite(block.data, 1, block.size, fout);
    }

    fclose(fout);
    return nullptr;
}
