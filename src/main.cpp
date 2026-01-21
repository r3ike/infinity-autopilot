#ifdef HAL_TEENSY
    #include <Arduino.h>
#elif defined(HAL_SITL)

#endif

#include <FlightController.hpp>
FlightController fc;

void setup() {
    fc.init();
}

void loop() {
    //MAI ESEGUITO CON RTOS
}
