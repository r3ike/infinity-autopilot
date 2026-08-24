#pragma once
#include "SRIMBSub.hpp"
#include "SRIMBWorkItemSub.hpp"
#include "ringbuffer/RingBuffer.hpp"
#include <array>
#include <zephyr/kernel.h>
#include <cstdint>

#define MAX_WORK_ITEM 32

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
    uint8_t updates_count{0};
};

    
template <typename T, uint8_t QUEUE_LEN>
class SRIMBQueueTopic
{
 
public:
    SRIMBQueueTopic():
        msg_queue_(QUEUE_LEN)
    {
        k_mutex_init(&mtx_);
        queue_len_ = QUEUE_LEN;
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


        submit_workitems();     // aggiungere il logging (level dbg) del numero di workitem inviati
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

    bool register_work_item(SRIMBWorkItemSub* sub){
        k_mutex_lock(&mtx_, K_FOREVER);
        if (workitem_count_ >= MAX_WORK_ITEM)
        {
            k_mutex_unlock(&mtx_);
            return false;
        }

        work_items_[workitem_count_++] = {
            .work_item = sub,
            .required_updates = 1,
            .updates_count = 0
        };
        k_mutex_unlock(&mtx_);
        return true;
        
    }

    bool register_work_item(SRIMBWorkItemSub* sub, uint8_t required_updates){
        k_mutex_lock(&mtx_, K_FOREVER);
        if (workitem_count_ >= MAX_WORK_ITEM)
        {
            k_mutex_unlock(&mtx_);
            return false;
        }

        work_items_[workitem_count_++] = {
            .work_item = sub,
            .required_updates = required_updates,
            .updates_count = 0
        };
        k_mutex_unlock(&mtx_);
        return true;
        
    }

    bool set_required_updates(SRIMBWorkItemSub* sub, uint8_t required_updates){
        k_mutex_lock(&mtx_, K_FOREVER);
        for (size_t i = 0; i < workitem_count_; i++)
        {
            WorkItemSchedule* w = &work_items_[i];
            if (w->work_item == sub)
            {
                w->required_updates = required_updates;
                w->updates_count = 0;
                k_mutex_unlock(&mtx_);
                return true;
            }
            
        }
        k_mutex_unlock(&mtx_);
        return false;
    }

    void unregister_work_item(){

    }



private:
    struct k_mutex mtx_{};

    RingBuffer<Topic<T>> msg_queue_;
    size_t queue_len_ {0};

    uint64_t generation_ {0};
    uint64_t oldest_generation_ {0};

    std::array<WorkItemSchedule, MAX_WORK_ITEM> work_items_ ;          // Array contenente i work item da chiamare al publish su un topic   
    std::size_t workitem_count_ {0};

    /**
     * Metodo privato per fare il submit dei workitem registrati.
     * il submit sarà effettuato solo se la condizione di invio è rispettata
     * Il metodo ritorna il numero di workitem inviati alla workqueue
     */
    uint8_t submit_workitems(){
        uint8_t submited_count = 0;
        for (size_t i = 0; i < workitem_count_; i++)
        {
            w->updates_count++;
            WorkItemSchedule* w = &work_items_[i];
            if (w->updates_count >= w->required_updates)
            {
                submit_single_workitems(w->work_item);
                submited_count++;
                w->updates_count=0;
            }
        }
        

        return submited_count;
    }

    void submit_single_workitems(SRIMBWorkItemSub* workitem){
        struct k_work_q* queue = workitem->getWorkQueue();

        if (queue)
        {
            k_work_submit_to_queue(queue, workitem->getWorkItem());
        }else{
            k_work_submit(workitem->getWorkItem());
        }
        
    }
};


} // namespace srimb





