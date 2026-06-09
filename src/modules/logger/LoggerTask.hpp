#pragma once

#include "Logger.hpp"
#include "ITask.hpp"

namespace uav::tasks
{
    
class LoggerTask : public scheduler::ITask
{
private:
    Logger _logger;

public:
    /**
     * TODO:
     *  - modifcare la configurazione mediante KCONFIG
     */
    static constexpr scheduler::TaskConfig taskConf {
        .name = "logger",
        .priority = 8,
        .policy = scheduler::SchedPolicy::RoundRobin,
        .period_us = 5000,
        -deadline_us = 5000,
        .stack_size = 2048
    }

    LoggerTask() : ITask(taskConf) {};
    ~LoggerTask() = default;


    bool init() override {
        
    }

    void run() override {
        _logger.log();
    }
    

    void stop() override {
        _logger_task.stop();
    }
};
} // namespace uav::tasks






