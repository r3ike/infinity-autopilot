#pragma once
#include "SRIMBSub.hpp"
#include "SRIMBWorkItemSub.hpp"
#include "ringbuffer/RingBuffer.hpp"
#include <array>
#include <zephyr/kernel.h>
#include <cstdint>

namespace srimb
{

template<typename T>
struct Topic
{
    T data;
    uint64_t generation = 0;
};

struct WorkItemSchedule
{
    SRIMBWorkItemSub* work_item;
    uint8_t required_updates{1};        // numero di update richiesti per il submit del workitem
    uint8_t updates_count{0};           // numero di update effettuati  => quando required_updates == updates_count il workitem viene inviato alla wq
};

    
template <typename T>
class SRIMBQueueTopic
{
 
public:
    SRIMBQueueTopic(size_t queue_len) :
        msg_queue_(queue_len) 
    {
        k_mutex_init(&mtx_);
        queue_len_ = queue_len;
    };


    ~SRIMBQueueTopic() {

    };

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

        k_mutex_unlock(&mtx_);
    }

    bool updated(const SRIMBSub& sub){
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
        
        
        uint32_t offset = (sub.get_last_generation() - oldest_generation_ + 1) % queue_len_;
        uint32_t tail_idx =  msg_queue_.get_tail_idx();

        uint32_t idx = (tail_idx + offset) % queue_len_;

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
    size_t queue_len_ {0};

    uint64_t generation_ {0};
    uint64_t oldest_generation_ {0};

    std::array<WorkItemSchedule, MAX_WORK_ITEM> work_items_ ;          // Array contenente i work item da chiamare al publish su un topic   
    std::size_t count_ {0};
};


} // namespace srimb





