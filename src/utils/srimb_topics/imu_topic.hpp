#pragma once
#include "modules/SRIMB/srimb_topic.hpp"
#include <cstdint>
#include <./utils/Vector3f.h>
#include <config/parameters.h>


struct ImuData {
    Vector3f raw_acc;
    Vector3f raw_gyro;

    //Imu dopo essere stato pre-filtrato => LPF, notch filter, temperature compensation, sculling e coning compensation
    Vector3f filtered_acc;          
    Vector3f filtered_gyro;

    int16_t temp;
    //Da aggiungere scoring e stato sensore e capire se mettere quaternioni qua(quindi se fare la conversione nel driver o nel imuManager)
};

extern SRIMB_Topic<ImuData> topic_imu[IMU_INSTANCES];
