#pragma once
#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>
#include <cstdint>
#include "srimb_topic.hpp"
#include "Vector3f.h"
#include "HAL.hpp"


struct ImuData {
    Vector3f raw_acc;
    Vector3f raw_gyro;

    //Imu dopo essere stato pre-filtrato => LPF, notch filter, temperature compensation, sculling e coning compensation
    Vector3f filtered_acc;          
    Vector3f filtered_gyro;

    double temp;
    uint8_t imu_id = 0;
    //Da aggiungere scoring e stato sensore e capire se mettere quaternioni qua(quindi se fare la conversione nel driver o nel imuManager)
};

extern SRIMB_Topic<ImuData> topic_imu[IMU_INSTANCES];
