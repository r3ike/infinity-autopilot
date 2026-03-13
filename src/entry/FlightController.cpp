#include <FlightController.hpp>



void FlightController::init()
{  
    init_work_queues();

    _imu_manager_task.init();


}

void FlightController::start()
{
    _imu_manager_task.start();
}




