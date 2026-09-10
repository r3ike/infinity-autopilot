#pragma once
#include "WorkItemBase.hpp"
#include "WorkQueue.hpp"

namespace srimb
{
class SRIMBTopicBase
{
public:
    ~SRIMBTopicBase() = default;

    virtual void publish(T& msg_data) = 0;
    virtual bool poll(SRIMBSub& sub, T& out) = 0;
    virtual bool updated(SRIMBSub& sub) = 0;

    virtual bool registerCallback(SRIMBSub& sub) = 0;
    virtual bool unregisterCallback(SRIMBSub& sub) = 0;

    
};
    
} // namespace srimb



