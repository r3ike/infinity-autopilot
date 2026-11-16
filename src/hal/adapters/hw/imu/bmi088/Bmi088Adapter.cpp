#include "Bmi088Adapter.hpp"
#include "registration/device_registration.hpp"
#include "registry/DeviceRegistry.hpp"

static void register_bmi088(DeviceRegistry& registry, HALContext& ctx) {

    #define REGISTER_BMI088_INST(inst)                                                                                             \
	do{                                                                                                                         \
	   const struct device *accel_device = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(inst), accel));                                 \
	   const struct device *gyro_device = DEVICE_DT_GET(DT_PHANDLE(DT_DRV_INST(inst), gyro));                                   \
	   registry->register_imu();               \
	}while(0)    

    DT_INST_FOREACH_STATUS_OKAY(REGISTER_BMI088_INST);
}

HAL_REGISTER_ADAPTER("infinity_autopilot_bmi088_imu", register_bmi088);
