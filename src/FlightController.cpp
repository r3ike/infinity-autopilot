#include <FlightController.hpp>



// Implementazione del Singleton
FlightController& FlightController::getInstance()
{
    // Questa variabile 'instance' viene creata solo la prima volta 
    // che chiami getInstance(). Le volte successive viene restituita quella già esistente.
    static FlightController instance; 
    return instance;
}

void FlightController::hardLoopWrapper()
{
    getInstance().hardLoop();
}



void FlightController::init()
{  
    _hal.init();

    _hal.getTimeInstance()->startHardLoop(LOOP_RATE_HARD_LOOP, FlightController::hardLoopWrapper);
    
    _imu_manager.init();
}


void FlightController::hardLoop()
{
    uint32_t hardloop_start = _hal.getTimeInstance()->micros();
    
    // Hard loop tasks

    _imu_manager.run();
    

    
}

void FlightController::runSoftLoop()
{
    uint32_t frame_start = _hal.getTimeInstance()->micros();


    // Soft loop tasks => il resto del frame usato per i soft tasks
    const uint32_t loop_period_us = hz_to_us(LOOP_RATE_HARD_LOOP);  //Conversione da hz a microsecondi

    while (_hal.getTimeInstance()->micros() - frame_start < loop_period_us)
    {
        /**
         * Soft-tasks:
         * - Telemetry
         * - Logging
         * - led toggling
         * - gps
         */
    }
    
}



