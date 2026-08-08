#pragma once
#include "RingBuffer.hpp"

template <typename data_type>
class RingBufferTimestamp : public RingBuffer<data_type>
{
private:
    /* data */
public:
    RingBufferTimestamp(size_t size): RingBuffer(size){}
    ~RingBufferTimestamp() {this->_deallocate();}

    bool pop_oldest(data_type &out){
        return this->pop(out);
    }

    /**
     * TODO:
     *  - pop_oldest
     *  - get_oldest => prende senza spostare la tail
     *  - get_newest
     *  - get_sample_timestamp => prende il sample filtrato dal timestamp => 2 prototipi di questo metodo uno senza interval che 
     *                            prende il sample con il timestamp esatto mentre con interval prende i sample compresi tra +- interval
     */

    bool get_oldest(data_type &out){
        uint32_t oldest_idx = this->get_tail_idx();

        return this->get(oldest_idx, out);
    }

    bool get_newest(data_type &out){
        uint32_t newest_idx = this->get_head_idx();

        return this->get(newest_idx, out);
    }


    void get_sample_timestamp(){}
};

