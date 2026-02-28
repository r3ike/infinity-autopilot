#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <hal/HAL.hpp>

#include <utils/Vector3f.h>
#include <utils/Quaternion.h>
#include <config/board_configs.h>
#include <config/parameters.h>

class ICM42688P_driver : public HAL_IMU
{
private:
    /* data */
public:
    ICM42688P_driver();
    ~ICM42688P_driver() = default;

    bool init() override;
    void calib() override;

    Vector3f getRawGyro() override;
    Vector3f getRawAccel() override;
    ImuData getRawImu() override;
};



