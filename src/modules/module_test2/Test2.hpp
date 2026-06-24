#pragma once

#include <cstdint>
#include "WorkItem.hpp"
#include "WorkQueue.hpp"
#include "SRIMB.hpp"
#include "SRIMBWorkItemSub.hpp"

#include "uav_types.hpp"

class Test2 : public WorkItemBase<Test2>, public SRIMBWorkItemSub
{
public:
    Test2(srimb::SRIMBTopic<ImuData>& topic) : imu_topic_(topic) {
        imu_topic_.register_work_item(this);
    };
    ~Test2() = default;

    
    // eseguito nella work queue
    void handler() {
        ImuData data;
        uint64_t timestamp;

        if (!imu_topic_.poll(*this, data, timestamp))
        {
            return;
        }

        //...
    }

    struct k_work* getWorkItem() override {
        return this.work_;
    }

    struct k_work_q* getWorkQueue() override {

    }
private:
    WorkQueue queue_ ;

    srimb::SRIMBTopic<ImuData>& imu_topic_;
};

