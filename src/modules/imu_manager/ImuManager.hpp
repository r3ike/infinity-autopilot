#pragma once

#include <zephyr/kernel.h>

#include "imu_preprocessor/ImuPreprocessor.hpp"
#include "SRIMB.hpp"
#include "uav_types.hpp"

#include "HAL_configs.hpp"



/**
 * Questa classe si occupa di gestire le IMU, in particolare è il modulo addetto a pubblicare il topic ImuData e 
 */

template <std::size_t NUM_IMU_INSTANCES>        // NUM_IMU_INSTANCES è il numero totale di instanze imu
class ImuManager
{
public:
    ImuManager(
        srimb::SRIMBTopic<RawAccData> (&raw_acc_topic)[NUM_IMU_INSTANCES],
        srimb::SRIMBTopic<RawGyroData> (&raw_gyro_topic)[NUM_IMU_INSTANCES]
    ) : 
    raw_imus_topic_(raw_imus_topic),
    imus_topic_(imus_topic)
    {};

    ~ImuManager() = default;

    void init();

private:
    srimb::SRIMBTopic<RawAccData>& raw_acc_topic_[NUM_IMU_INSTANCES];
    srimb::SRIMBTopic<RawGyroData>& raw_gyro_topic_[NUM_IMU_INSTANCES];

    ImuPreprocessor preprocessors_ [NUM_IMU_INSTANCES];
};


