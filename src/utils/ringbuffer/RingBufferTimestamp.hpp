#pragma once
#include "RingBuffer.hpp"

template <typename data_type>
class RingBufferTimestamp : public RingBuffer<data_type>
{
private:
    /* data */
public:
    RingBufferTimestamp(size_t size): RingBuffer(size);
    ~RingBufferTimestamp() {this->_deallocate();};

    void pop_oldest(){}

};

