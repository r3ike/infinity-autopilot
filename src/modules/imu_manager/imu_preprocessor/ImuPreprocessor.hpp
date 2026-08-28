#pragma once

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <cstdint>

#include "Loggable.hpp"
#include "WorkItem.hpp"
#include "WorkQueue.hpp"
#include "SRIMB.hpp"
#include "SRIMBQueueTopic.hpp"
#include "SRIMBWorkItemSub.hpp"

#include "data_types/DataTypes.h"

#include "srimb_topics/TopicTypes.h"

/**
 * Classe che si occupa di effettuare il preprocessing dei sample delle imu:
 *  - filtering (LPF. notche, ecc..)
 *  - calibration
 *  - checks (...)
 */
class ImuPreprocessor : public WorkItemBase<ImuPreprocessor>, public srimb::SRIMBWorkItemSub, public Loggable
{

public:
    ImuPreprocessor(srimb::RawGyroTopic &raw_gyro_topic, srimb::RawAccTopic &raw_acc_topic):
        raw_gyro_topic_(raw_gyro_topic),
        raw_acc_topic_(raw_acc_topic)
            {
                raw_gyro_topic_.register_work_item(this);
            };
    ~ImuPreprocessor() = default;

    void handler(){

    }

    struct k_work* getWorkItem() override {
        return &this->work_;
    }

    struct k_work_q* getWorkQueue() override {
        return nullptr;
    }

private:
    srimb::RawGyroTopic& raw_gyro_topic_;
    srimb::RawAccTopic& raw_acc_topic_;

    srimb::SRIMBSub gyro_sub_;
    srimb::SRIMBSub acc_sub_;
};

