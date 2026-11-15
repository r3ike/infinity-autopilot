#pragma once
#include "SRIMBTopicBase.hpp"

namespace srimb
{
template <typename TopicType>
class SRIMBPub
{
public:
	using DataType = typename TopicType::DataType;

	SRIMBPub() = default;
	explicit SRIMBPub(SRIMBTopicBase* base_topic): topic_(static_cast<TopicType*>(base_topic)) {};

	~SRIMBPub() = default;

	void setTopic(SRIMBTopicBase* base_topic){
		topic_ = static_cast<TopicType*>(base_topic);
	};

	bool publish(const DataType& msg_data){
		if (!topic_) {
            return false;
        }
        topic_->publish(msg_data);   // publish() ritorna void
        return true;
	};


private:
	TopicType* topic_{nullptr};	
};
} // namespace srimb





