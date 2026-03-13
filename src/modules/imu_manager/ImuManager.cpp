
#include "ImuManager.hpp"

void ImuManager::init()
{
    
}

void ImuManager::run()
{
    for (size_t i = 0; i < IMU_INSTANCES; i++)
    {
        _publish_single_imu(i);
    }
    
}

void ImuManager::_publish_single_imu(uint8_t instance)
{
    ImuData imu;
    uint64_t now = 0;

    imu = _hal.get_imu_instance(instance)->get_imu();

    imu.imu_id = instance;


    srimb_publish(topic_imu[instance], imu, now);
}
