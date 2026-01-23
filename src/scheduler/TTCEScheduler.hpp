#pragma once
#include <cstdint>
#include <hal/HAL.hpp>


/**
 * Time-Triggered Cyclic Executive hard
 */

 class TTCEScheduler
 {
 private:
    void (*callbackHardTasks)();        //callback function per le tasks dell'hardloop
    uint32_t _frame_us;                 //dimensione del frame in microsecondi
    HAL* _hal;

 public:
    TTCEScheduler(HAL* hal);
    ~TTCEScheduler();

    void addHardTasks(void (*callbackHardTasks)());         //funzione per aggiungere le tasks dell'hardloop

    void tick();
 };
 


