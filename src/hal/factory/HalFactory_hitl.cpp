#ifdef CONFIG_TARGET_HITL

#include "HalFactory.hpp"
#include "registry/DeviceRegistry.hpp"


std::unique_ptr<DeviceRegistry> make_hal_for_target() {
	auto reg = std::make_unique<DeviceRegistry>();

	return reg;
}

#endif
