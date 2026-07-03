#pragma once

#include <zephyr/kernel.h>
#include <cstdint>

namespace infinity::scheduler
{

enum class SchedPolicy : uint8_t {
    EDF,
    FixedPriority,
    RoundRobin,
    Cooperative
};

struct TaskConfig
{
    const char* name;
    int priority;
    SchedPolicy policy;
    uint32_t period_us;
    uint32_t deadline_us;
    std::size_t stack_size;
};

class ITask
{
public:
    explicit ITask(const TaskConfig& cfg) : _cfg(cfg) {};

    ITask(const ITask&)            = delete;
    ITask& operator=(const ITask&) = delete;
    virtual ~ITask() = default;

    virtual bool init() = 0;
    virtual void run() = 0;
    virtual void onFailsafe() {}

    const TaskConfig& taskConfig(){return _cfg;}
    const char* taskName(){return _cfg.name;}
    const int taskPriority(){return _cfg.priority;}
    k_tid_t taskTid(){return _tid;}

protected:
    void refreshDeadline() const {
        if (_cfg.policy == SchedPolicy::EDF && _tid) {
            k_thread_deadline_set(_tid, static_cast<int>(_cfg.deadline_us));
        }
    }

private:
    TaskConfig _cfg;
    k_tid_t _tid {nullptr};
    friend class Scheduler;
};

} // namespace infinity::scheduler
