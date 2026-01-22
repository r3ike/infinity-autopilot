#pragma once
#include <hal/HAL.hpp>


class HAL_IMU_SITL : public HAL_IMU{
private:
    /* data */
public:
    HAL_IMU_SITL();
    ~HAL_IMU_SITL();

    virtual bool init() override;
    virtual void calib() override;
    virtual Vector3f readGyro() override;
    virtual Vector3f readAccel() override;
    virtual ImuData read() override;
};

class HAL_MOTOR_SITL : public HAL_MOTOR
{
private:
    /* data */
public:
    HAL_MOTOR_SITL();

    void write(float m1, float m2, float m3, float m4) override;
};