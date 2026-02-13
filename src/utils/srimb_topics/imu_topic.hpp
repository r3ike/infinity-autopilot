#pragma once
#include "../srimb_topic.hpp"

constexpr uint8_t IMU_INSTANCES = 1;

struct ImuData {
    float ax, ay, az;
    float gx, gy, gz;
    int16_t temp;
};

extern SRIMB_Topic<ImuData> topic_imu[IMU_INSTANCES];
