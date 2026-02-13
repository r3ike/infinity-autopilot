#include "modules/ImuManager/ImuManager.hpp"
#include "ImuManager.hpp"



void ImuManager::init()
{
    /**
     * LPF initialization 
     */
    _imu_lpf_filter.init_lpf_acc(LOOP_RATE_HARD_LOOP, ACC_CUTOFF_FREQ);
    _imu_lpf_filter.init_lpf_gyro(LOOP_RATE_HARD_LOOP, GYRO_CUTOFF_FREQ);
}

void ImuManager::publish()
{
}

void ImuManager::_publish_single_imu(uint8_t instance)
{
    ImuData imu;

    _hal.getImuInstance(instance)->getRawImu();

    uint64_t now = micros();
    
    srimb_publish(topic_imu[instance], imu, now);
}
