// hal_factory.hpp
#pragma once
#include <memory>

class DeviceRegistry;

std::unique_ptr<DeviceRegistry> make_hal_for_target();
