#pragma once
#include "WorkItemBase.hpp"
#include "WorkQueue.hpp"

namespace srimb
{
class SRIMBTopicBase
{
public:
    ~SRIMBTopicBase() = default;

    virtual bool publishRaw(const void* data, size_t size) = 0;
    virtual bool readRaw(void* out_buffer, size_t size) = 0;

    virtual bool updated(SRIMBSub& sub) = 0;

    virtual bool registerCallback(SRIMBSub& sub) = 0;
    virtual bool unregisterCallback(SRIMBSub& sub) = 0;

    
};
    
} // namespace srimb



