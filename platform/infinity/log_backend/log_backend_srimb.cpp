#include "SRIMBTopic.hpp"
#include "log_backend.h"  

extern "C" void srimb_log_publish(void *ctx, const LogMessage *msg, uint64_t timestamp) {
    auto *topic = static_cast<SRIMBTopic<LogMessage>*>(ctx);
    if (topic) {
        topic->publish(*msg, timestamp);
    }
}