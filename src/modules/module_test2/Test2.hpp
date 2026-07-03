#pragma once
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <cstdint>
#include "WorkItem.hpp"
#include "WorkQueue.hpp"
#include "SRIMB.hpp"
#include "SRIMBWorkItemSub.hpp"

#include "uav_types.hpp"

class Test2 : public WorkItemBase<Test2>, public srimb::SRIMBWorkItemSub
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

        if (!imu_topic_.poll(imu_sub_, data, timestamp))
        {
            printk("no update");
            return;
        }

        printk("Msg sul topic imu data arrivato: %u, %u", data.imu_id, timestamp);
    }

    struct k_work* getWorkItem() override {
        return &this->work_;
    }

    
private:
    srimb::SRIMBSub imu_sub_;
    srimb::SRIMBTopic<ImuData>& imu_topic_;
};

