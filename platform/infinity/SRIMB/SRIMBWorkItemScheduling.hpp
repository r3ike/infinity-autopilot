#pragma once

/**
 * Classe che viene estesa di SRIMBTopic e SRIMBQueueTopic per permettere
 * la gestione delle callback tramite workitem
 */

#define MAX_WORK_ITEM 32

namespace srimb
{


struct WorkItemCallback
{
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
        for (size_t i = 0; i < workitem_count_; i++)
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
    SRIMBWorkItemScheduling(/* args */);
    ~SRIMBWorkItemScheduling() = default;


    bool registerWorkItemCb(WorkItemBase* wi){
        k_mutex_lock(&mtx_, K_FOREVER);
        if (count_ >= MAX_WORK_ITEM)
        {
            return false;
        }

        work_items_cb[count_++] = WorkItemCallback cb = {
                                        .work_item = wi, 
                                        .work_queue = nullptr,
                                        .required_updates = 1,
                                        .updates_count = 0
                                    };
        k_mutex_unlock(&mtx_);
        return true;
    }

    bool registerWorkItemCb(WorkItemBase* wi,  WorkQueue* wq){
        k_mutex_lock(&mtx_, K_FOREVER);
        if (count_ >= MAX_WORK_ITEM)
        {
            return false;
        }

        work_items_cb[count_++] = WorkItemCallback cb = {
                                        .work_item = wi, 
                                        .work_queue = wq,
                                        .required_updates = 1,
                                        .updates_count = 0
                                    };
        k_mutex_unlock(&mtx_);
        return true;
    }

    bool unregisterWorkItemCb(){}
};
    
} // namespace srimb




