#ifdef CONFIG_TARGET_TEENSY41
#include "HalFactory.hpp"

#include "registration/device_registration.hpp"
#include "registry/DeviceRegistry.hpp"

std::unique_ptr<HAL> make_hal_for_target(srimb::SRIMBTopicManager& tm, WorkQueue& wq){
	auto registry = std::make_unique<DeviceRegistry>();

	// Scorre TUTTI i record depositati nella sezione
    STRUCT_SECTION_FOREACH(hal_driver_registrar, reg) {
        reg->register_fn(*registry, ctx);   // ogni driver si registra da solo
    }

	return registry;
}


#endif
