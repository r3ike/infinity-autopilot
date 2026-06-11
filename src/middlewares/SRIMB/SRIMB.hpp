#pragma once
#include "srimb_topic.hpp"
#include "SRIMBWorkItemSub.hpp"
#include <array>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/**
     * TODO:
     *  - rimuovere valore hard coded usare kconfig?
     */
#define MAX_WORK_ITEM 16

namespace srimb
{

template<typename T>
class SRIMBTopic
{

public:
    SRIMB() {
        k_mutex_init(&mtx_);
    };
    ~SRIMB() {};

    
    void publish(SRIMB_Topic& msg) {
        k_mutex_lock(&mtx_, K_FOREVER);

        topic_ = msg;

        k_mutex_unlock(&mtx_);

        submit_all_workitems();
    }

    /**
     * TODO:
     *  - capire se aggiungere il passaggio della struttura dati del sub per 
     *    capire se il messaggio e nuovo o vecchio
     */
    void poll(){

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

    SRIMB_Topic<T> topic_ {};

    /**
     * TODO:
     *  - capire se si può togliere la classe SRIMBWorkItemSub e usare solo quella workitem
     */
    std::array<SRIMBWorkItemSub*, MAX_WORK_ITEM> work_items_ ;          // Array contenente i work item da chiamare al publish su un topic   
    std::size_t count_ {0};

    struct k_mutex mtx_{};
};

} // namespace srimb




