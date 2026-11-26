#include "Bmi088Adapter.hpp"
#include "registration/device_registration.hpp"
#include "registry/DeviceRegistry.hpp"

#include "Bmi088_config.hpp"

//#include "log_instance_pool.hpp"
//LOG_INSTANCE_POOL_DEFINE(imu_bmi088_device, NUM_BMI088_INSTANCES);

Bmi088Adapter::Bmi088Adapter(std::string_view device_name, const struct device *accel_dev, const struct device *gyro_dev) : 
	device_name_(device_name),
	bmi088_gyro_("bmi088_gyro_device", gyro_dev),
	bmi088_acc_("bmi088_accel_device", accel_dev)
{
	mark_device_state(DeviceState::Uninitialized);
}

bool Bmi088Adapter::init(uint8_t instance_id, HalContext &ctx)
{
	mark_device_state(DeviceState::Initializing);
	assign_identity(instance_id, device_name_);

	bool isGyroOk = bmi088_gyro_.init(instance_id, ctx);
	bool isAccOk = bmi088_acc_.init(instance_id, ctx);

	if ((!isGyroOk && bmi088_gyro_.state() == DeviceState::Failed) || (!isAccOk && bmi088_acc_.state() == DeviceState::Failed))
	{
		mark_device_state(DeviceState::Failed);
		return false;
	}
	
	if (isAccOk && isGyroOk && bmi088_gyro_.state() == DeviceState::Ready && bmi088_acc_.state() == DeviceState::Ready)
	{
		mark_device_state(DeviceState::Ready);
	}

    return true;
}

bool Bmi088Adapter::start() {
    if (!is_ready()) return false;

    if (!bmi088_gyro_.start()) {
        mark_device_state(DeviceState::Failed);
        return false;
    }
    if (!bmi088_acc_.start()) {
        bmi088_gyro_.stop();                 // rollback: non lasciare il gyro acceso
        mark_device_state(DeviceState::Failed);
        return false;
    }
    mark_device_state(DeviceState::Started);
    return true;
}

bool Bmi088Adapter::stop() {
    if (!is_started()) return false;
    bmi088_acc_.stop();
    bmi088_gyro_.stop();
    mark_device_state(DeviceState::Ready);

	return true;
}



/**
 * ADAPTER SELF-REGISTRATION
 */
#ifdef CONFIG_BMI088_DRIVER_ENABLED

#define REGISTER_BMI088_INST(inst)                                                                                             \
do{                                                                                                                         \
   const struct device *accel_device = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(inst), accel));                                 \
   const struct device *gyro_device = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(inst), gyro));                                   \
   registry.>register_imu(std::make_unique<Bmi088Adapter>("bmi088", accel_device, gyro_device));               \
}while(0)  

static void register_bmi088(DeviceRegistry& registry) {

    DT_INST_FOREACH_STATUS_OKAY(REGISTER_BMI088_INST);
}

HAL_REGISTER_ADAPTER("infinity_autopilot_bmi088_imu", register_bmi088);

#endif
