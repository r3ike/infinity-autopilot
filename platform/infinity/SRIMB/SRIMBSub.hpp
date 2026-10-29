#pragma once 
#include <cstdint>

#include "SRIMBTopicBase.hpp"
#include "SRIMBTopic.hpp"

namespace srimb
{

    /**
     * TODO:
     *  - aggiungere subscriber name
     */

struct WorkItemCallback
{
    WorkItemBase* work_item {nullptr};
    WorkQueue* work_queue {nullptr};
    uint8_t required_updates{1};        // numero di update richiesti per il submit del workitem
    uint8_t updates_count{0};
};

template <typename TopicType>
class SRIMBSub
{

public:
	using DataType = TopicType::DataType;

    explicit SRIMBSub(SRIMBTopicBase* base_topic) 
        // Conversione sicura da base a derivata concreta
        : typed_topic_(static_cast<TopicType*>(base_topic)) {}

    ~SRIMBSub() = default;
    
    void set_last_generation(uint64_t gen) {last_generation_ = gen;}
    uint64_t get_last_generation() {return last_generation_;}
    
    void set_missed(bool missed) {missed_ = missed;}
    bool get_missed() {return missed_;}

	void publish(const DataType& msg_data){
		return topic_ ? topic_->publish(msg_data) : false;
	};

    bool poll(DataType& out){
		
		return topic_->poll(this, out);
	};

    bool updated() {
		return topic_->updated(this);
	};

    bool registerCallback() = 0;
    bool unregisterCallback() = 0;

private:
    uint64_t last_generation_{0};

    bool missed_ {false}; // flag to warn about a missed message in the queue topic

	TopicType* topic_{nullptr};	
};

} // namespace srimb


