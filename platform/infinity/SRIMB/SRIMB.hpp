#pragma once
#include <zephyr/kernel.h>
#include <cstdint>

#include <unordered_map>
#include <vector>
#include <string>   // TODO: cercare di togliere allocazione dinamnica

#include "SRIMBTopicBase.hpp"
#include "SRIMBSub.hpp"

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

    SRIMBTopicBase& getTopic(const char *name, uint8_t instance_id);

private:
    
    std::unordered_map<std::string, std::vector<SRIMBTopicBase>> topics_ ;
};

} // namespace srimb



