#ifndef DOCUMENTS_BOUND_BUFFER_H
#define DOCUMENTS_BOUND_BUFFER_H

#include <vector>
namespace data_structures {
template<typename ValueType>
class BoundBuffer {
public:

private:
    const int max_size_;
    int size_;
    int nextIn_;
    int nextOut_;
    typedef std::vector<int> BufferList;
    typedef std::unique_ptr<BufferList[]> storage_;

public:
    explicit BoundBuffer(int max_size) : max_size_(max_size) {}
    // AKA head, front, etc
    int nextIn() {
        return (nextIn_);
    }
    // AKA tail, back, etc
    int nextOut() {
        return (nextOut_);
    }

    bool isEmpty() {
        if (size() == 0){
            return true;
        }
        else {
            return false;
        }
    }

    int size() {
        return size_;
    }


    void addLast(const ValueType& value) {
        //BufferList &list = storage_.begin(); //, storage_.end());
        

        nextIn_ = value;
        nextIn_ = (nextIn() + 1) % size();
    }

    ValueType removeFirst() {
        // check if queue is empty

        return ValueType();
    }
};

} // namespace data_structures

#endif //DOCUMENTS_BOUND_BUFFER_H

// used the following link for conceptual help
// https://github.com/uu-os-2018/module-4/blob/master/mandatory/src/bounded_buffer.c
// https://stackoverflow.com/questions/15278343/c11-thread-safe-queue