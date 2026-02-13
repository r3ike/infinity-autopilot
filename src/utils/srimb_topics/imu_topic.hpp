#pragma once
#include "modules/SRIMB/srimb_topic.hpp"
#include <cstdint>
#include <./utils/Vector3f.h>
#include <config/parameters.h>


struct ImuData {
    Vector3f acc;
    Vector3f gyro;
    int16_t temp;
};

extern SRIMB_Topic<ImuData> topic_imu[IMU_INSTANCES];
