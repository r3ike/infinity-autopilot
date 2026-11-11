#pragma once
#include "SRIMBTopicBase.hpp"

template <typename TopicType>
class SRIMBPub
{
public:
	using DataType = typename TopicType::DataType;

	SRIMBPub(){};
	explicit SRIMBPub(SRIMBTopicBase* base_topic): topic_(static_cast<TopicType*>(base_topic)) {};

	~SRIMBPub() = default;

	void setTopic(SRIMBTopicBase* base_topic){
		topic_ = static_cast<TopicType*>(base_topic);
	};

	bool publish(const DataType& msg_data){
		return topic_ ? topic_->publish(msg_data) : false;
	};


private:
	TopicType* topic_{nullptr};	
};


