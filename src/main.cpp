#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <signal.h>
#include "bounded_buffer.hpp"
#include "producer.hpp"
#include "consumer.hpp"

volatile int keep_running = 1;

void sigint_handler(int) {
    keep_running = 0;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <input> <output> <key>\n";
        return 1;
    }

    const char* input_path  = argv[1];
    const char* output_path = argv[2];
    char        key         = argv[3][0];

    // Регистрируем обработчик SIGINT через sigaction
    struct sigaction sa{};
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);

    // Создаём общий буфер
    BoundedBuffer buffer;

    // Аргументы для потоков
    ProducerArgs prod_args{ &buffer, input_path, &keep_running };
    ConsumerArgs cons_args{ &buffer, output_path, key };

    // Запускаем потоки
    pthread_t producer_tid, consumer_tid;
    pthread_create(&producer_tid, nullptr, producer_thread, &prod_args);
    pthread_create(&consumer_tid, nullptr, consumer_thread, &cons_args);

    // Ждём завершения обоих потоков
    pthread_join(producer_tid, nullptr);
    pthread_join(consumer_tid, nullptr);

    std::cout << "Done.\n";
    return 0;
}
