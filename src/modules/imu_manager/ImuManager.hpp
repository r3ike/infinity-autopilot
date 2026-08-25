#pragma once

#include <zephyr/kernel.h>
#include <cstddef>
#include <cstdint>
#include "imu_preprocessor/ImuPreprocessor.hpp"
#include "SRIMB.hpp"
#include "SRIMBQueueTopic.hpp"
#include "data_types/DataTypes.h"

#include "srimb_topics/TopicTypes.h"

#include "HAL_configs.hpp"



/**
 * Questa classe si occupa di gestire le IMU, in particolare è il modulo addetto a pubblicare il topic ImuData e 
 */

template <size_t NUM_IMU_INSTANCES>        // NUM_IMU_INSTANCES è il numero totale di instanze imu
class ImuManager
{
public:
    ImuManager(
        srimb::RawAccTopic (&raw_acc_topic)[NUM_IMU_INSTANCES],
        srimb::RawGyroTopic (&raw_gyro_topic)[NUM_IMU_INSTANCES]
    ) : 
    raw_acc_topic_(raw_acc_topic),
    raw_gyro_topic_(raw_gyro_topic)
    {
        for(int i = 0; i < NUM_IMU_INSTANCES; i++){
            preprocessors_[count_++] = std::make_unique<ImuPreprocessor>(raw_gyro_topic_[i], raw_acc_topic_[i]);
        }
    };

    ~ImuManager() = default;

    void init() {
        for(int i = 0; i < NUM_IMU_INSTANCES; i++){

        }
    };

private:
    srimb::RawAccTopic (&raw_acc_topic_)[NUM_IMU_INSTANCES];
    srimb::RawGyroTopic (&raw_gyro_topic_)[NUM_IMU_INSTANCES];

    std::array<std::unique_ptr<ImuPreprocessor>, NUM_IMU_INSTANCES> preprocessors_;
    size_t count_ {0};
};


