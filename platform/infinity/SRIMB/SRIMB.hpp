#pragma once

#include <cstdint>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>   // TODO: cercare di togliere allocazione dinamnica

#include "SRIMBTopicBase.hpp"
#include "SRIMBSub.hpp"

#include <zephyr/kernel.h>

namespace srimb
{
    
/**
 * La classe SRIMB si occupa di gestire in modo centralizzato tutti i topic
 */
class SRIMBTopicManager
{

public:
    SRIMBTopicManager(/* args */);
    ~SRIMBTopicManager();

	template <typename TopicType>
    SRIMBTopicBase* getOrCreateTopic(uint8_t instance_id);

private:
    std::mutex mutex_;
    std::unordered_map<std::string, st::unique_ptr<SRIMBTopicBase>> topics_ ;
};

template <typename TopicType>
inline SRIMBTopicBase *SRIMBTopicManager::getOrCreateTopic(uint8_t instance_id)
{
	std::lock_guard<std::mutex> lock(mutex_);

	std::string lookup_key = std::string(TopicType::TOPIC_NAME) + "_" + std::to_string(instance_id);

    auto it = topics_.find(lookup_key);
    if (it != topics_.end()) {
        return it->second.get();
    }

	// Se il topic non è presente ne instanzio uno nuovo
	TopicType new_topic = std::make_unique<TopicType>();
	TopicType raw_ptr = new_topic.get();
	topic_[lookup_key] = std::move(new_topic);

	return raw_ptr;
}

} // namespace srimb
