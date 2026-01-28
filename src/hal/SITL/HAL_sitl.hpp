#pragma once
#include <hal/HAL.hpp>

#include <chrono>
#include <thread>
#include <fstream>
#include <mutex>
#include <iostream>


class HAL_IMU_SITL : public HAL_IMU
{
private:
    /* data */
public:
    HAL_IMU_SITL();
    ~HAL_IMU_SITL() = default;

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
    ~HAL_MOTOR_SITL() = default;

    void write(float m1, float m2, float m3, float m4) override;
};

class HAL_TIME_INTERRUPS_SITL : public HAL_Time_Interrupts
{
private:
    std::thread _workerThread;
    std::mutex _mutex;
    bool _running = true;
public:
    HAL_TIME_INTERRUPS_SITL();
    ~HAL_TIME_INTERRUPS_SITL() = default;

    unsigned long long micros() override;

    void enterCritical() override { _mutex.lock(); }
    void exitCritical() override { _mutex.unlock(); }

    void startHardLoop(uint32_t frequency, void (*callback)()) override;
};


