#include <FlightController.hpp>



FlightController::FlightController() {}
FlightController::~FlightController(){}

void FlightController::init()
{
    // Fase di init
    hal.init();
    scheduler = new TTCEScheduler(&hal);
    
    scheduler->addHardTasks();

}

void hardLoopTasks()
{
    //Tasks dell'hard loop
}

void run(){

}


