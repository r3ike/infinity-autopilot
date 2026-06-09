#pragma once
#include "srimb_topic.hpp"
#include <array>

namespace srimb
{

template<typename T>
class SRIMBTopic
{

public:
    SRIMB();
    ~SRIMB();

    void publish() {
        notify_all();
    }

    void poll(){

    }

    void register_work_item(){

    }

    void unregister_callback(){

    }

private:
    SRIMB_Topic<T> _topic;

    // aggiungere array con le callback

    
};

} // namespace srimb




