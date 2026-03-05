#pragma once

#include <pthread.h>
#include <cstring>
#include <cstddef>

constexpr int BLOCK_SIZE  = 4096;
constexpr int BUFFER_CAP  = 8;

struct Block {
    char   data[BLOCK_SIZE];
    size_t size;
    bool   is_eof;
};

class BoundedBuffer {
public:
    BoundedBuffer() : head_(0), tail_(0), count_(0) {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&not_full_,  nullptr);
        pthread_cond_init(&not_empty_, nullptr);
    }

    ~BoundedBuffer() {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&not_full_);
        pthread_cond_destroy(&not_empty_);
    }

    void push(const Block& block) {
        pthread_mutex_lock(&mutex_);
        while (count_ == BUFFER_CAP)
            pthread_cond_wait(&not_full_, &mutex_);
        buf_[tail_] = block;
        tail_ = (tail_ + 1) % BUFFER_CAP;
        ++count_;
        pthread_cond_signal(&not_empty_);
        pthread_mutex_unlock(&mutex_);
    }

    Block pop() {
        pthread_mutex_lock(&mutex_);
        while (count_ == 0)
            pthread_cond_wait(&not_empty_, &mutex_);
        Block block = buf_[head_];
        head_ = (head_ + 1) % BUFFER_CAP;
        --count_;
        pthread_cond_signal(&not_full_);
        pthread_mutex_unlock(&mutex_);
        return block;
    }

private:
    Block          buf_[BUFFER_CAP];
    int            head_, tail_, count_;
    pthread_mutex_t mutex_;
    pthread_cond_t  not_full_;
    pthread_cond_t  not_empty_;
};
