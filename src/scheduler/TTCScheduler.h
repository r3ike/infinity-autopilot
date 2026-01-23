#pragma once
#include <Arduino.h>
#include <utils/Timer.h>

/*
Time-Triggered Cooperative scheduler

Struct per le task, ogni task è caratterizzata da una funzione di callback che viene chiamata qunado 
è ora di eseguire il task, da un intervallo in microsecondi che definisce la frequenza di esecuzione 
e un timer che viene creato per ogni task.
*/
struct Task
{
    void (*callback)();
    uint32_t interval_us;
    Timer timer;
};


class TTCScheduler
{
private:
    uint8_t _numTask;
    static const uint8_t _NUM_MAX_TASKS = 10;
    Task _tasks[_NUM_MAX_TASKS];

public:
    TTCScheduler() : _numTask(0) {};

    void addTask(void (*callback)(), uint32_t interval_us){
        if (_numTask < _NUM_MAX_TASKS)
        {
            _tasks[_numTask] = {callback, interval_us, Timer()};
            _numTask++;
        }
    }

    void run(){
        for (uint8_t i = 0; i < _numTask; i++)
        {
            if (_tasks[i].timer.elapsedMicros(_tasks[i].interval_us))
            {
                _tasks[i].callback();
            }
        }
    }
};


