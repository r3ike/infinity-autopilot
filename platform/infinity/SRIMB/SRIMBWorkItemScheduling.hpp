#pragma once

/**
 * Classe che viene estesa di SRIMBTopic e SRIMBQueueTopic per permettere
 * la gestione delle callback tramite workitem
 */

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
    /* data */

protected:
    /**
     * Metodo protetto per fare il submit dei workitem registrati.
     * il submit sarà effettuato solo se la condizione di invio è rispettata
     * Il metodo ritorna il numero di workitem inviati alla workqueue
     */
    uint8_t submit_workitems(){
        uint8_t submitted_count = 0;
        for (size_t i = 0; i < workitem_count_; i++)
        {
            WorkItemSchedule* w = &work_items_[i];
            w->updates_count++;
            if (w->updates_count >= w->required_updates)
            {
                submit_single_workitem(w->work_item);
                submitted_count++;
                w->updates_count=0;
            }
        }
        

        return submitted_count;
    }

    void submit_single_workitem(WorkItemCallback& cb){
        WorkQueue* wq = cb.work_queue;

        if (wq)
        {
            cb.work_item->submitTo(*wq);
        }else{
            cb.work_item->submit();
        }
    }

public:
    SRIMBWorkItemScheduling(/* args */);
    ~SRIMBWorkItemScheduling() = default;
};
    
} // namespace srimb




