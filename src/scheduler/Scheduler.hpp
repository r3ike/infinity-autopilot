#pragma once

#include "ITask.hpp"
#include <array>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


namespace infinity::scheduler
{

// Struct con le info dei singoli thread 
struct TaskEntry {
    ITask* task { nullptr };
    k_thread_stack_t* stack { nullptr };
    std::size_t stack_size { 0 };
    struct k_thread tcb {};
    bool initialized{ false };
};
    
class Scheduler
{
public:
    Scheduler() = default;
    ~Scheduler();

    bool addTask(ITask* task, k_thread_stack_t* stack, std::size_t stack_size){
        if (_count >= CONFIG_MAX_TASKS)
        {
            // Logging
            return false;
        }
        
        _tasks_entries[_count] = {
            .task = task,
            .stack = stack,
            .stack_size = stack_size
        };

        _count++;

        return true;
    }

    /**
     * Metodo che inizializza tutte le task
     */
    bool initAllTasks(){
        bool status = true;
        for (size_t i = 0; i < _count; i++)
        {
            auto& e = _task_entries[i]
            e.initialized = e.task->init();
            if (!e.initialized)
            {
                // Logging 
                status = false;
            }
        }
        return status;
    }

    void start(){
        for (std::size_t i = 0; i < _count; i++)
        {
            auto& e = _tasks_entries[i];

            if (!e.initialized){
                continue;
            }


            
        }
    }

    void notifyFailSafe(){}

    void suspendAll(){
        for (std::size_t i = 0; i < _count; i++)
        {
            if (_tasks_entries[i].initialized && _tasks_entries[i].task->_tid)
            {
                k_thread_suspend(_tasks_entries[i].task->_tid);
            }
            
        }
        
    }

    void resumeAll(){
        for (std::size_t i = 0; i < _count; i++)
        {
            if (_tasks_entries[i].initialized && _tasks_entries[i].task->_tid)
            {
                k_thread_resume(_tasks_entries[i].task->_tid);
            }
        }
    }

    std::size_t TaskCount(){return _count;}
    
private:
    static void threadEntry(void* arg1, void*, void*) {
        auto* task = static_cast<ITask*>(arg1);
        while (true) task->run();
    }

    std::array<TaskEntry, CONFIG_MAX_TASKS> _tasks_entries {};
    std::size_t _count {0};  // Numero di entri effettivamente inserite nell'array
};

} // namespace infinity::scheduler
