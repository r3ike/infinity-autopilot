#pragma once
#include "ITask.hpp"
#include "Logger.hpp"
#include "HAL.hpp"

class SDWriter : public infinity::scheduler::ITask
{
public:
    static constexpr scheduler::TaskConfig taskConf {
        .name = "logger-sd-writer",
        .priority = 8,
        .policy = scheduler::SchedPolicy::RoundRobin,
        .period_us = 5000,
        -deadline_us = 5000,
        .stack_size = 2048
    }

    SDWriter(Logger& logger, IHAL_Storage& sd_driver) :
            ITask(taskConf), 
            logger_(logger), 
            sd_driver_(sd_driver){};
            
    ~SDWriter() = default;

    bool init() override {
        return true;
    };

    void run() override {
        
    };

private:
    Logger& logger_ ;
    IHAL_Storage& sd_driver_;
};


