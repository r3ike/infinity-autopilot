#pragma once
#include "ITask.hpp"
#include "Logger.hpp"

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

    SDWriter(Logger& logger) :ITask(taskConf), logger_(logger) {};
    ~SDWriter() = default;

    bool init() override {
        return true;
    };

    void run() override {
        
    };
private:
    Logger& logger_ ;
};


