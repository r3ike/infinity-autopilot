#pragma once

template <typename TopicType>
class SRIMBPub
{
public:
	using DataType = TopicType::DataType;

	explicit SRIMBPub(SRIMBTopicBase* base_topic): typed_topic_(static_cast<TopicType*>(base_topic)) {};

	~SRIMBPub() = default;

	void publish(const DataType& msg_data){
		return topic_ ? topic_->publish(msg_data) : false;
	};


private:
	TopicType* topic_{nullptr};	
};


