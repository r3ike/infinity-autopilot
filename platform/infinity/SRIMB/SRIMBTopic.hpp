#pragma once
#include <cstdint>
#include "SRIMBSub.hpp"
#include "WorkItemBase.hpp"
#include "WorkQueue.hpp"
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

struct WorkItemCallback
{
    WorkItemBase* work_item {nullptr},
    WorkQueue* work_queue {nullptr}
};
    

template<typename T>
class SRIMBTopic
{

public:
    SRIMBTopic() {
        k_mutex_init(&mtx_);
    };
    ~SRIMBTopic() {};

    
    void publish(T& msg_data) {
        k_mutex_lock(&mtx_, K_FOREVER);

        data_ = msg_data;
        generation_++;

        k_mutex_unlock(&mtx_);

        submit_all_workitems();
    }

    
    bool poll(SRIMBSub& sub, T& out){
        k_mutex_lock(&mtx_, K_FOREVER);
        if(sub.get_last_generation() == generation_){
            k_mutex_unlock(&mtx_);
            return false;
        }

        out = data_;
        sub.set_last_generation(generation_);

        k_mutex_unlock(&mtx_);

        return true;
    }

    bool updated(SRIMBSub& sub){
        k_mutex_lock(&mtx_, K_FOREVER);
        if(sub.get_last_generation() == generation_){
            k_mutex_unlock(&mtx_);
            return false;
        }

        k_mutex_unlock(&mtx_);

        return true;
    }

    bool register_work_item(WorkItemBase* wi){
        if (count_ >= MAX_WORK_ITEM)
        {
            return false;
        }

        work_items_cb[count_++] = WorkItemCallback cb = {.work_item = wi, .work_queue = nullptr };
        return true;
        
    }

    bool register_work_item(WorkItemBase* wi, WorkQueue* wq){
        if (count_ >= MAX_WORK_ITEM)
        {
            return false;
        }

        work_items_cb[count_++] = WorkItemCallback cb = {.work_item = wi, .work_queue = wq };
        return true;
        
    }

    void unregister_work_item(){

    }

private:
    
    void submit_all_workitems(){
        for (size_t i = 0; i < count_; i++)
        {
            WorkItemCallback& cb = work_items_cb[i];
            submit_single_workitems(cb);
        }
        
    }

    void submit_single_workitems(WorkItemCallback& cb){
        WorkQueue* wq = cb.work_queue;

        if (wq)
        {
            cb.work_item->submitTo(*wq);
        }else{
            cb.work_item->submit();
        }
    }

    T data_ {};
    uint64_t generation_ {0};   // generation id of the msg

    /**
     * TODO:
     *  - capire se si può evitare di usare il puntatore ma il reference
     */
    
    WorkItemCallback work_items_cb [MAX_WORK_ITEM];          // Array contenente i work item da chiamare al publish su un topic   
    size_t count_ {0};

    struct k_mutex mtx_{};
};

} // namespace srimb




