#ifndef DOCUMENTS_BOUND_BUFFER_H
#define DOCUMENTS_BOUND_BUFFER_H

/*
 * used the following links for reference
 * ref 1:
 * https://stackoverflow.com/questions/15798222/is-this-a-correct-way-to-implement-a-bounded-buffer-in-c
 * ref 2:
 * https://codereview.stackexchange.com/questions/171360/thread-safe-bounded-buffer-fifo-queue-using-c11
 */

#include <mutex>
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <vector>

#include "data_structures/queue/queue.h"

namespace data_structures {
template<typename ValueType>
class BoundBuffer {

private:
    pthread_mutex_t mutex;
    sem_t empty;
    sem_t full;

    const int max_size_;
    int size_;
    std::vector<int> storage_;
    Queue<int> q;


public:

    explicit BoundBuffer(int max_size) : max_size_(max_size) {pthread_mutex_init(&mutex, nullptr);
        sem_init(&empty, 0, max_size);
        sem_init(&full, 0, 0);
    }

    int size() {
        return size_;

    }

    void addLast(const ValueType& value) {
        //while buffer is full
        while (size()==max_size_) {
            // empty -> down();
            int sem_trywait(sem_t *empty);
            // mutex -> down();
            int pthread_mutex_trylock(pthread_mutex_t *mutex);
            // int item = produce_item();
            q.addLast(value);
            int pthread_mutex_unlock(pthread_mutex_t *mutex);
            int sem_trywait(sem_t *full);




        }
    }

    ValueType removeFirst(){

        while (size()==0){
            //int sem_trylock(semt_t *full);
            int sem_trywait(sem_t *full);
            // mutex locking, waiting, and notification structure from ref 1
            //std::unique_lock<std::mutex> lock(mutex)
            int pthread_mutex_trylock(pthread_mutex_t *mutex);
            // while buffer is empty
            q.removeFirst();
            int pthread_mutex_tryunlock(pthread_mutex_t *mutex);
        }
        // remove item
        // decrement size
        size_--;
        // notify threads waiting on full buffer
        //nonfull.notify_all();
        return ValueType();
    }


};
} // namespace data_structures

#endif //DOCUMENTS_BOUND_BUFFER_H
