#pragma once

#include <cstdint>

#include "SRIMB.hpp"
#include "SRIMBSub.hpp"

#include "Loggable.hpp"
#include "WorkItem.hpp"
#include "WorkQueue.hpp"

#include "data_types/DataTypes.h"
#include "srimb_topics/TopicTypes.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


/**
 * Classe che si occupa di effettuare il preprocessing dei sample delle imu:
 *  - filtering (LPF. notche, ecc..)
 *  - calibration
 *  - checks (...)
 */
class ImuPreprocessor : public WorkItem<ImuPreprocessor>, public Loggable
{

public:
    ImuPreprocessor(uint8_t instance_id, srimb::SRIMBTopicManager& topicManager):
        gyro_sub_(topicManager.getOrCreateTopic<srimb::RawGyroTopic>(instance_id)),
        acc_sub_(topicManager.getOrCreateTopic<srimb::RawAccTopic>(instance_id))
    {
        gyro_sub_.registerCallback(*this);
    };
    
    ~ImuPreprocessor() = default;

    void handler(){

    }


private:
    srimb::SRIMBSub<srimb::RawGyroTopic> gyro_sub_;
    srimb::SRIMBSub<srimb::RawAccTopic> acc_sub_;
};

