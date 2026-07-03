#pragma once
#include <cstdint>
#include "SRIMBSub.hpp"
#include "SRIMBWorkItemSub.hpp"
#include <array>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/**
     * TODO:
     *  - rimuovere valore hard coded usare kconfig?
     *  - capire se è meglio usare uno shared mutex al posto che un mutex normale
     */
#define MAX_WORK_ITEM 32

namespace srimb
{

template<typename T>
class SRIMBTopic
{

public:
    SRIMBTopic() {
        k_mutex_init(&mtx_);
    };
    ~SRIMBTopic() {};

    
    void publish(T& msg_data, uint64_t timestamp) {
        k_mutex_lock(&mtx_, K_FOREVER);

        data_ = msg_data;
        timestamp_ = timestamp;
        generation_++;

        k_mutex_unlock(&mtx_);

        submit_all_workitems();
    }

    
    bool poll(SRIMBSub& sub, T& out, uint64_t& timestamp){
        k_mutex_lock(&mtx_, K_FOREVER);
        if(sub.get_last_generation() == generation_){
            return false;
        }

        out = data_;
        timestamp = timestamp_;
        sub.set_last_generation(generation_);

        k_mutex_unlock(&mtx_);

        return true;
    }

    bool updated(SRIMBSub& sub){
        k_mutex_lock(&mtx_, K_FOREVER);
        if(sub.get_last_generation() == generation_){
            return false;
        }

        k_mutex_unlock(&mtx_);

        return true;
    }

    bool register_work_item(SRIMBWorkItemSub* sub){
        if (count_ >= MAX_WORK_ITEM)
        {
            return false;
        }

        work_items_[count_++] = sub;
        return true;
        
    }

    void unregister_callback(){

    }

private:
    
    void submit_all_workitems(){
        for (size_t i = 0; i < count_; i++)
        {
            SRIMBWorkItemSub* s = work_items_[i];
            submit_single_workitems(s);
        }
        
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

    T data_ {};
    uint64_t timestamp_ {0};    // timestamp of the msg
    uint64_t generation_ {0};   // generation id of the msg

    /**
     * TODO:
     *  - capire se si può togliere la classe SRIMBWorkItemSub e usare solo quella workitem
     */
    std::array<SRIMBWorkItemSub*, MAX_WORK_ITEM> work_items_ ;          // Array contenente i work item da chiamare al publish su un topic   
    std::size_t count_ {0};

    struct k_mutex mtx_{};
};

} // namespace srimb




