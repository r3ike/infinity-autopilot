#include "Bmi088Adapter.hpp"
#include "registration/device_registration.hpp"
#include "registry/DeviceRegistry.hpp"

Bmi088Adapter::Bmi088Adapter(std::string_view model, const struct device *accel_dev, const struct device *gyro_dev) : 
	model_name_(model),
	bmi088_gyro_()
{

}

bool Bmi088Adapter::init(uint8_t instance_id, HalContext &ctx)
{
	assign_identity(instance_id, model_name_);
    return false;
}



/**
 * ADAPTER SELF-REGISTRATION
 */
static void register_bmi088(DeviceRegistry& registry, HALContext& ctx) {

    #define REGISTER_BMI088_INST(inst)                                                                                             \
	do{                                                                                                                         \
	   const struct device *accel_device = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(inst), accel));                                 \
	   const struct device *gyro_device = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(inst), gyro));                                   \
	   registry->register_imu();               \
	}while(0)    

    DT_INST_FOREACH_STATUS_OKAY(REGISTER_BMI088_INST(infinity_autopilot_bmi088_imu));
}

HAL_REGISTER_ADAPTER("infinity_autopilot_bmi088_imu", register_bmi088);


