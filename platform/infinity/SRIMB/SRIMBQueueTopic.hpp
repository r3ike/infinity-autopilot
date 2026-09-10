#pragma once

#include "ringbuffer/RingBuffer.hpp"
#include <array>
#include <zephyr/kernel.h>
#include <cstdint>

#include "SRIMBSub.hpp"
#include "SRIMBTopicBase.hpp"
#include "SRIMBWorkItemScheduling.hpp"

#define MAX_WORK_ITEM 32

namespace srimb
{

template<typename T>
struct Topic
{
    T data;
    uint64_t generation = 0;
};



    
template <typename T, uint8_t QUEUE_LEN>
class SRIMBQueueTopic
{
 
public:
    SRIMBQueueTopic():
        msg_queue_(QUEUE_LEN)
    {
        k_mutex_init(&mtx_);
    };

    ~SRIMBQueueTopic() = default;

    void publish(T& msg_data){
        k_mutex_lock(&mtx_, K_FOREVER);

        msg_queue_.push({
            .data = msg_data,
            .generation = ++generation_
        });

        //update oldest generation
        uint32_t tail_idx =  msg_queue_.get_tail_idx();
        Topic<T> msg;
        msg_queue_.get(tail_idx, msg);
        oldest_generation_ = msg.generation;


        submitWorkitems();     // aggiungere il logging (level dbg) del numero di workitem inviati
        k_mutex_unlock(&mtx_);
    }

    bool updated(SRIMBSub& sub){
        k_mutex_lock(&mtx_, K_FOREVER);
        bool has_new = sub.get_last_generation() < generation_;
        k_mutex_unlock(&mtx_);
        return has_new;
    }

    bool poll(SRIMBSub& sub, T& out){
        k_mutex_lock(&mtx_, K_FOREVER);

        if (sub.get_last_generation() == generation_)
        {
            k_mutex_unlock(&mtx_);
            return false;
        }

        if (sub.get_last_generation() + 1 < oldest_generation_)
        {
            sub.set_missed(true);
            sub.set_last_generation(oldest_generation_ - 1);
        }
        
        
        uint32_t offset = (sub.get_last_generation() - oldest_generation_ + 1) % QUEUE_LEN;
        uint32_t tail_idx =  msg_queue_.get_tail_idx();

        uint32_t idx = (tail_idx + offset) % QUEUE_LEN;

        Topic<T> msg;
        msg_queue_.get(idx, msg);

        out = msg.data;

        sub.set_last_generation(msg.generation);
        k_mutex_unlock(&mtx_);
        return true;
    }

 


private:
    struct k_mutex mtx_{};

    RingBuffer<Topic<T>> msg_queue_;

    uint64_t generation_ {0};
    uint64_t oldest_generation_ {0};

};


} // namespace srimb





