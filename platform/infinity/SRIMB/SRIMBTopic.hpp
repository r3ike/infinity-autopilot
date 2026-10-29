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
    

template<typename TopicMetadata>
class SRIMBTopic : public SRIMBWorkItemScheduling, public SRIMBTopicBase
{

public:
	using DataType = typename TopicMetadata::DataType

	static constexpr const char* NAME = TopicMetadata::TOPIC_NAME;
    static constexpr uint32_t HASH = TopicMetadata::TOPIC_HASH;

    SRIMBTopic() {
        k_mutex_init(&mtx_);
    };
    ~SRIMBTopic() {};

    
    void publish(DataType& msg_data) override {
        k_mutex_lock(&mtx_, K_FOREVER);

        data_ = msg_data;
        generation_++;

        k_mutex_unlock(&mtx_);

        submitWorkitems();
    }

    
    bool poll(SRIMBSub& sub, DataType& out) override{
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

    DataType data_ {};
    uint64_t generation_ {0};   // generation id of the msg

};

} // namespace srimb




