#include <hal/SITL/HAL_sitl.hpp>
#include "HAL_sitl.hpp"

HAL_IMU_SITL::HAL_IMU_SITL(){}


bool HAL_IMU_SITL::init()
{
    return false;
}

void HAL_IMU_SITL::calib()
{
}

Vector3f HAL_IMU_SITL::readGyro()
{
    return {0,0,0};
}

Vector3f HAL_IMU_SITL::readAccel(){
    return {0,0,0};
}

ImuData HAL_IMU_SITL::read()
{
    return ImuData();
}

HAL_MOTOR_SITL::HAL_MOTOR_SITL()
{
}

void HAL_MOTOR_SITL::write(float m1, float m2, float m3, float m4)
{
}
