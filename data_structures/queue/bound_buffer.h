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
#include <condition_variable>
#include <thread>
#include <iostream>

#include "data_structures/queue/queue.h"

std::mutex mx;
std::condition_variable nonfull;
std::condition_variable nonempty;

namespace data_structures {
template<typename ValueType>
class BoundBuffer {

public:


private:
    const int max_size_;
    int size_;

public:

    explicit BoundBuffer(int max_size) : max_size_(max_size) {
    }

    int size() {
        return size_;
    }

    void addLast(const ValueType& value) {
        // mutex locking and waiting structure from ref 1
        // lock mutex mx before entering critical section
        std::unique_lock<std::mutex> lock(mx);
        // while buffer is full
        while (size()==max_size_){
            // wait && release mx
            nonfull.wait(lock);
        }
        // do add

    }

    ValueType removeFirst() {
        // mutex locking, waiting, and notification structure from ref 1
        std::unique_lock<std::mutex> lock(mx);
        // while buffer is empty
        while (size()==0){
            // wait && release mx
            nonempty.wait(lock);
        }
        // remove item

        // notify threads waiting on full buffer
        nonfull.notify_all();
        return ValueType();
    }


};

} // namespace data_structures

#endif //DOCUMENTS_BOUND_BUFFER_H
