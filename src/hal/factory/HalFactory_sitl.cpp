#ifdef CONFIG_TARGET_SITL
#include "HalFactory.hpp"
#include "registry/DeviceRegistry.hpp"


std::unique_ptr<HAL> make_hal_for_target(srimb::SRIMBTopicManager& tm, WorkQueue& wq) {
	auto reg = std::make_unique<DeviceRegistry>();

	return reg;
}

#endif
