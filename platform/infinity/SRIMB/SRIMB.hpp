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
    SRIMBTopicManager(){
		k_mutex_init(&mtx_);
	};
    ~SRIMBTopicManager() = default;

	template <typename TopicType>
    SRIMBTopicBase* getOrCreateTopic(uint8_t instance_id);

private:
    struct k_mutex mtx_{};
    std::unordered_map<std::string, std::unique_ptr<SRIMBTopicBase>> topics_ ;
};

template <typename TopicType>
inline SRIMBTopicBase *SRIMBTopicManager::getOrCreateTopic(uint8_t instance_id)
{
	k_mutex_lock(&mtx_, K_FOREVER);

	std::string lookup_key = std::string(TopicType::TOPIC_NAME) + "_" + std::to_string(instance_id);

    auto it = topics_.find(lookup_key);
    if (it != topics_.end()) {
		k_mutex_unlock(&mtx_);
        return it->second.get();
    }

	// Se il topic non è presente ne instanzio uno nuovo
	auto new_topic = std::make_unique<TopicType>();
	TopicType* raw_ptr = new_topic.get();
	topics_[lookup_key] = std::move(new_topic);

	k_mutex_unlock(&mtx_);
	return raw_ptr;
}

} // namespace srimb
