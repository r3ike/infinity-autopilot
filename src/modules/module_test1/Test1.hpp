#pragma once

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "ITask.hpp"
#include "SRIMB.hpp"
#include "uav_types.hpp"

LOG_MODULE_REGISTER(test1, LOG_LEVEL_DBG);

namespace infinity::tasks
{

class Test1 : public scheduler::ITask
{
private:
    srimb::SRIMBTopic<ImuData>& imu_topic_;
public:
    static constexpr scheduler::TaskConfig taskConf {
        .name = "test1",
        .priority = 8,
        .policy = scheduler::SchedPolicy::RoundRobin,
        .period_us = 1000000,
        -deadline_us = 1000000,
        .stack_size = 2048
    };

    Test1(srimb::SRIMBTopic<ImuData>& topic) : ITask(taskConf), imu_topic_(topic) {};
    ~Test1() = default;

    bool init() override {
        LOG_INF("Task test1 avviato con successo:");
        return true;
    };

    void start() override {

    };
};    

} // namespace name





