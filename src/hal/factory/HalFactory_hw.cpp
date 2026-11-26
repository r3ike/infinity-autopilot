#ifdef CONFIG_TARGET_TEENSY41
#include "HalFactory.hpp"
#include "HalContext.hpp"
#include "registration/device_registration.hpp"
#include "registry/DeviceRegistry.hpp"

std::unique_ptr<DeviceRegistry> make_hal_for_target(HalContext ctx){
	auto registry = std::make_unique<DeviceRegistry>(ctx);

	// Scorre TUTTI i record depositati nella sezione
    STRUCT_SECTION_FOREACH(HalDriverRegistrar, reg) {
        reg->register_fn(*registry);   // ogni driver si registra da solo
    }

	return registry;
}


#endif
