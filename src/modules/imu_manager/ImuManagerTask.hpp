#pragma once

#include "Module.hpp"
#include "PeriodicTask.hpp"
#include "Scheduler.hpp"

#include "ImuManager.hpp"

class ImuManagerTask : public Module
{
private:
    PeriodicTask _imu_manager_task;

    ImuManager _imu_manager;
public:
    ImuManagerTask(HAL& hal) : _imu_manager_task([this]() { this->run(); }, fast_wq, K_MSEC(10)), _imu_manager(hal) {};
    ~ImuManagerTask() = default;

    void init() override {
        _imu_manager.init();
    };

    void start() override{
        _imu_manager_task.start();
    };


    void stop() override{
        _imu_manager_task.stop();
    }

    void run() override {
        _imu_manager.run();
    }
};

