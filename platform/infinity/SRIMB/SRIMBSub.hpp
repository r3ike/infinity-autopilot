#pragma once 
#include <cstdint>
#include "WorkItemBase.hpp"
#include "WorkQueue.hpp"
#include "SRIMBTopicBase.hpp"

namespace srimb
{



template <typename TopicType>
class SRIMBSub
{

public:
	using DataType = typename TopicType::DataType;
	using Self = SRIMBSub<TopicType>;

	SRIMBSub(){};
    explicit SRIMBSub(SRIMBTopicBase* base_topic) 
        // Conversione sicura da base a derivata concreta
        : topic_(static_cast<TopicType*>(base_topic)) {}

    ~SRIMBSub() = default;

	void setTopic(SRIMBTopicBase* base_topic){
		topic_ = static_cast<TopicType*>(base_topic);
	};
    
    void set_last_generation(uint64_t gen) {last_generation_ = gen;}
    uint64_t get_last_generation() {return last_generation_;}
    
    void set_missed(bool missed) {missed_ = missed;}
    bool get_missed() {return missed_;}

	
    bool poll(DataType& out){
		return topic_ ? topic_->poll(*this, out) : false;
	}

	bool updated(){
		return topic_ ? topic_->updated(*this) : false;
	}

    bool registerCallback(WorkItemBase& wi){
		return topic_->registerWorkItemCb<Self>(*this, wi);
	};

	bool registerCallback(WorkItemBase& wi, WorkQueue& wq){
		return topic_->registerWorkItemCb<Self>(*this, wi, wq);
	};

	// TODO: da finire
    bool unregisterCallback(){
		return topic_->unregisterWorkItemCb<Self>();
	};

private:
    uint64_t last_generation_{0};

    bool missed_ {false}; // flag to warn about a missed message in the queue topic

	TopicType* topic_{nullptr};	
};

} // namespace srimb


