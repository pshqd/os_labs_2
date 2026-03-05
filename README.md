# Простой файловый копировщик с
шифрованием

Многопоточный файловый копировщик с XOR-шифрованием.
Использует схему producer/consumer (POSIX threads), bounded buffer и libcaesar.so из Lab 1.

## Build

    make

## Run

    ./securecopy input.txt output.txt K

- input.txt  — входной файл
- output.txt — зашифрованный выходной файл
- K          — ключ шифрования (один символ, XOR)

Для расшифровки используй тот же ключ повторно:

    ./securecopy output.txt decoded.txt K

## Stop

    Ctrl+C  # graceful shutdown — потоки корректно завершаются

## Clean

    make clean
