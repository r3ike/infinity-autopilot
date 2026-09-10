#pragma once
#include <cstdint>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "SRIMBTopicBase.hpp"
#include "SRIMBWorkItemScheduling.hpp"
#include "SRIMBSub.hpp"


/**
     * TODO:
     *  - rimuovere valore hard coded usare kconfig?
     *  - capire se è meglio usare uno shared mutex al posto che un mutex normale
     */


namespace srimb
{

struct WorkItemCallback
{
    WorkItemBase* work_item {nullptr},
    WorkQueue* work_queue {nullptr}
};
    

template<typename T>
class SRIMBTopic : public SRIMBWorkItemScheduling, public SRIMBTopicBase
{

public:
    SRIMBTopic() {
        k_mutex_init(&mtx_);
    };
    ~SRIMBTopic() {};

    
    void publish(T& msg_data) override {
        k_mutex_lock(&mtx_, K_FOREVER);

        data_ = msg_data;
        generation_++;

        k_mutex_unlock(&mtx_);

        submitWorkitems();
    }

    
    bool poll(SRIMBSub& sub, T& out) override{
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

    bool updated(SRIMBSub& sub) override{
        k_mutex_lock(&mtx_, K_FOREVER);
        if(sub.get_last_generation() == generation_){
            k_mutex_unlock(&mtx_);
            return false;
        }

        k_mutex_unlock(&mtx_);

        return true;
    }


private:

    T data_ {};
    uint64_t generation_ {0};   // generation id of the msg

};

} // namespace srimb




