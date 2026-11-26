#include "DeviceRegistry.hpp"
#include "log_instance_pool.hpp"

LOG_INSTANCE_POOL_DEFINE(device_registry, 1)


DeviceRegistry::DeviceRegistry(HalContext& ctx)  : ctx_(ctx)
{
	init_logging(device_registry_log_instance_get(0));
}


bool DeviceRegistry::register_imu(std::unique_ptr<IImu> imu)
{
	if (imu_count_ >= CONFIG_MAX_IMU_DEVICE) { return false; }
	imu_instances_[imu_count_++] = std::move(imu);
    return true;
}

bool DeviceRegistry::register_gnss(std::unique_ptr<IGnss> gnss)
{
    if (gnss_count_ >= CONFIG_MAX_GNSS_DEVICE) { return false; }
	gnss_instances_[gnss_count_++] = std::move(gnss);
    return true;
}

bool DeviceRegistry::init_all()
{
    return false;
}

/**
 * PRIVATE METHOD
 */

bool DeviceRegistry::start_all_imu()
{
	for (size_t id = 0; id < imu_count_; id++)
	{
		imu_instances_[id].get()->init(id, ctx_);
	}
	
    return false;
}
