#include <FlightController.hpp>



FlightController::FlightController() {}
FlightController::~FlightController(){}

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
    // Fase di init
    hal.init();

    hal.time->startHardLoop(LOOP_RATE_HARD_LOOP, FlightController::hardLoopWrapper);

}


void FlightController::hardLoop()
{
    // Hard loop tasks

    ImuData imuData = hal.imu->read();
}

void FlightController::runSoftLoop()
{
    uint32_t frame_start = hal.time->micros();

    

    // Soft loop tasks => il resto del frame usato per i soft tasks
    const uint32_t loop_period_us = hz_to_us(LOOP_RATE_HARD_LOOP);  //Conversione da hz a microsecondi

    while (hal.time->micros() - frame_start < loop_period_us)
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



