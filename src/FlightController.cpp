#include <FlightController.hpp>



FlightController::FlightController() {}
FlightController::~FlightController(){}

void FlightController::init()
{
    // Fase di init
    hal.init();

    

}

void FlightController::run()
{
    uint32_t frame_start = hal.time->micros();

    // Hard loop tasks

    ImuData imuData = hal.imu->read();

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



