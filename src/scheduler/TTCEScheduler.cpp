#include <scheduler/TTCEScheduler.hpp>
#include "TTCEScheduler.hpp"

 TTCEScheduler::TTCEScheduler(HAL* hal){
    _hal = hal; 
 }
 TTCEScheduler::~TTCEScheduler(){}

 void TTCEScheduler::addHardTasks(void (*callbackHardTasks)()){
 }
 void TTCEScheduler::tick()
 {
    uint32_t start_frame = _hal->time->micros();

    callbackHardTasks();
 }