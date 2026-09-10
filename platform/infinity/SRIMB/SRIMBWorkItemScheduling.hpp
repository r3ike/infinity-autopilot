#pragma once
#include <zephyr/kernel.h>
#include <cstdint>

#include "SRIMBSub.hpp"
#include "WorkItemBase.hpp"
#include "WorkQueue.hpp"

/**
 * Classe che viene estesa di SRIMBTopic e SRIMBQueueTopic per permettere
 * la gestione delle callback tramite workitem
 */

#define MAX_WORK_ITEM 32

namespace srimb
{

struct WorkItemCallback
{
    SRIMBSub* topic_sub {nullptr};
    WorkItemBase* work_item {nullptr};
    WorkQueue* work_queue {nullptr};
    uint8_t required_updates{1};        // numero di update richiesti per il submit del workitem
    uint8_t updates_count{0};
};

class SRIMBWorkItemScheduling
{
private:
    WorkItemCallback work_items_cb [MAX_WORK_ITEM];          // Array contenente i work item da chiamare al publish su un topic   
    size_t cb_count_ {0};

    struct k_mutex mtx_{};

    void submit_single_workitem(WorkItemCallback& cb){
        WorkQueue* wq = cb.work_queue;

        if (wq)
        {
            cb.work_item->submitTo(*wq);
        }else{
            cb.work_item->submit();
        }
    }

protected:
    /**
     * Metodo protetto per fare il submit dei workitem registrati.
     * il submit sarà effettuato solo se la condizione di invio è rispettata
     * Il metodo ritorna il numero di workitem inviati alla workqueue
     */
    uint8_t submitWorkitems(){
        uint8_t submitted_count = 0;
        for (size_t i = 0; i < cb_count_; i++)
        {
            WorkItemCallback& cb = work_items_cb[i];
            cb.updates_count++;
            if (cb.updates_count >= cb.required_updates)
            {
                submit_single_workitem(cb);
                submitted_count++;
                cb.updates_count=0;
            }
        }
    
        return submitted_count;
    }

    

public:
    SRIMBWorkItemScheduling(){
        k_mutex_init(&mtx_);
    };
    ~SRIMBWorkItemScheduling() = default;


    bool registerWorkItemCb(SRIMBSub& sub, WorkItemBase& wi){
        k_mutex_lock(&mtx_, K_FOREVER);
        if (cb_count_ >= MAX_WORK_ITEM)
        {
            k_mutex_unlock(&mtx_);
            return false;
        }

        work_items_cb[cb_count_++] = WorkItemCallback cb = {
                                        .topic_sub = &sub,
                                        .work_item = &wi, 
                                        .work_queue = nullptr,
                                        .required_updates = 1,
                                        .updates_count = 0
                                    };
        k_mutex_unlock(&cb_count_);
        return true;
    }

    bool registerWorkItemCb(SRIMBSub& sub, WorkItemBase& wi,  WorkQueue& wq){
        k_mutex_lock(&mtx_, K_FOREVER);
        if (count_ >= MAX_WORK_ITEM)
        {
            k_mutex_unlock(&mtx_);
            return false;
        }

        work_items_cb[count_++] = WorkItemCallback cb = {
                                        .topic_sub = &sub,
                                        .work_item = &wi, 
                                        .work_queue = &wq,
                                        .required_updates = 1,
                                        .updates_count = 0
                                    };
        k_mutex_unlock(&mtx_);
        return true;
    }

    bool unregisterWorkItemCb(){
        return true;
    }

    bool set_required_updates(SRIMBSub& sub, uint8_t required_updates){
        k_mutex_lock(&mtx_, K_FOREVER);
        for (size_t i = 0; i < cb_count_; i++)
        {
            WorkItemCallback& cb = work_items_cb[i];
            if (cb.topic_sub == &sub)
            {
                cb.required_updates = required_updates;
                cb.updates_count = 0;
                k_mutex_unlock(&mtx_);
                return true;
            }
            
        }
        k_mutex_unlock(&mtx_);
        return false;
    }

};
    
} // namespace srimb




