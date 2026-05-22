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

    /**
     * TODO:
     *  - pop_oldest
     *  - get_oldest => prende senza spostare la tail
     *  - get_newest
     *  - get_sample_timestamp => prende il sample filtrato dal timestamp => 2 prototipi di questo metodo uno senza interval che 
     *                            prende il sample con il timestamp esatto mentre con interval prende i sample compresi tra +- interval
     */
};

