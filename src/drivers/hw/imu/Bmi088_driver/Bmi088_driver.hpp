#pragma once

/*-------------------------------
    WRAPPER PER LA IMU
-------------------------------*/
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <HAL.hpp>
#include <Vector3f.h>
#include <Quaternion.h>
// #include <config/board_configs.h>
// #include <config/parameters.h>
#include <srimb_topics/imu_topic/imu_topic.hpp>

// Ottieni il device tree node label e il device pointer per l'accelerometro
#define ACCEL_NODE DT_NODELABEL(bmi088_accel)

// Ottieni il device tree node label e il device pointer per il giroscopio
#define GYRO_NODE DT_NODELABEL(bmi088_gyro)


class Bmi088_driver : public HAL_IMU
{
private:
    Vector3f _gyro_rate_calib;
    
    const struct device *gyro_dev = DEVICE_DT_GET(GYRO_NODE);
    const struct device *accel_dev = DEVICE_DT_GET(ACCEL_NODE);

    double _getImuTemp(); 
    
public:
    Bmi088_driver();
    ~Bmi088_driver() = default;

    bool init() override;
    void calib() override;

    Vector3f getRawGyro() override;
    Vector3f getRawAccel() override;
    ImuData getRawImu() override;

    
};
