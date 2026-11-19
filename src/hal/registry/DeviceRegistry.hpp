#pragma once
#include <cstdint>
#include <memory>
#include <array>

#include "interfaces/IImu.hpp"
#include "interfaces/IGps.hpp"
#include "interfaces/IBaro.hpp"
#include "interfaces/IDevice.hpp"
#include "interfaces/ILidar.hpp"
#include "interfaces/IStorage.hpp"
#include "interfaces/IMag.hpp"

#include "Loggable.hpp"

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>

/**
 * Registro contente tutte le instanze registrate degli adattori
 */

class DeviceRegistry
{
	
public:
	DeviceRegistry(/* args */);
	~DeviceRegistry();

	bool register_imu(std::unique_ptr<IImu> imu);

	bool init_all_devices();

private:
	std::array<std::unique_ptr<IImu>, CONFIG_MAX_IMU_DEVICE> imu_instances_;
	uint8_t imu_count_{0};
};


