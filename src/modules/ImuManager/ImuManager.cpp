#include "modules/ImuManager/ImuManager.hpp"
#include "ImuManager.hpp"

void ImuManager::init()
{
    /**
     * LPF initialization
     */
    for (size_t i = 0; i < IMU_INSTANCES; i++)
    {
        _imu_lpf_filters.at(i)->init_lpf_acc(LOOP_RATE_HARD_LOOP, ACC_CUTOFF_FREQ);
        _imu_lpf_filters.at(i)->init_lpf_gyro(LOOP_RATE_HARD_LOOP, GYRO_CUTOFF_FREQ);
    }
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
    uint64_t now = _hal.getTimeInstance()->micros();

    imu = _hal.getImuInstance(instance)->getRawImu();

    _imu_lpf_filters.at(instance)->apply(imu);

    srimb_publish(topic_imu[instance], imu, now);
}
