#pragma once

#include "Scheduler.hpp"

#include "ImuManager.hpp"

class ImuManagerTask
{
private:
    ImuManager _imu_manager;


public:
    ImuManagerTask(HAL& hal) : _imu_manager(hal) {};
    ~ImuManagerTask() = default;

};

