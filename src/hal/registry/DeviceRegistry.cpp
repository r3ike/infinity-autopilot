#include "DeviceRegistry.hpp"

DeviceRegistry::DeviceRegistry(/* args */)
{
}

DeviceRegistry::~DeviceRegistry()
{
}

bool DeviceRegistry::register_imu(std::unique_ptr<IImu> imu)
{
	if (imu_count_ >= CONFIG_MAX_IMU_DEVICE) { return false; }
	
	imu_instances_[imu_count_++] = std::move(imu);

    return true;
}
