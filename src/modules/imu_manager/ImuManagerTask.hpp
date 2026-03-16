#pragma once

#include "Module.hpp"
#include "PeriodicTask.hpp"
#include "Scheduler.hpp"

#include "ImuManager.hpp"

class ImuManagerTask : public IModule
{
private:
    PeriodicTask _imu_manager_task;

    ImuManager _imu_manager;

    static void run_callback(void* context) {
        static_cast<ImuManagerTask*>(context)->run();
    }

public:
    ImuManagerTask(HAL& hal) : _imu_manager_task(run_callback, this, &fast_wq, K_MSEC(10)), _imu_manager(hal) {};
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

