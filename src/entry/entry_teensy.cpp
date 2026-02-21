
#include <Arduino.h>


#include <FlightController.hpp>



void setup() {
    FlightController& fc_instance = FlightController::getInstance();

    fc_instance.init();
}

void loop() {
    FlightController& fc_instance = FlightController::getInstance();
    fc_instance.runSoftLoop();
}
