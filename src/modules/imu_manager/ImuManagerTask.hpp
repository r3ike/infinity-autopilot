#pragma once

#include "Module.hpp"
#include "PeriodicWork.hpp"
#include "Scheduler.hpp"

#include "ImuManager.hpp"

class ImuManagerTask : public Module
{
private:
    PeriodicWork _work;

    ImuManager _imu_manager;
public:
    ImuManagerTask(HAL& hal) : _work(*this, medium_wq, K_MSEC(50)), _imu_manager(hal) {};
    ~ImuManagerTask() = default;

    void init() override {
        _imu_manager.init();
    };

    void start() override{
        _work.start();
    };

    void run() override {
        _imu_manager.run();
    }
};

