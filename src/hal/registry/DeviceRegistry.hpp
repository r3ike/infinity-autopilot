#pragma once
#include <cstdint>
#include <memory>
#include <array>

#include "interfaces/IImu.hpp"
#include "interfaces/IGnss.hpp"
#include "interfaces/IBaro.hpp"
#include "interfaces/IDevice.hpp"
#include "interfaces/ILidar.hpp"
#include "interfaces/IStorage.hpp"
#include "interfaces/IMag.hpp"

#include "factory/HalContext.hpp"

#include "Loggable.hpp"

#include <zephyr/kernel.h>
#include <zephyr/autoconf.h>

/**
 * Registro contente tutte le instanze registrate degli adattori
 */


class DeviceRegistry : public Loggable
{
	
public:
	DeviceRegistry(HalContext& ctx);
	~DeviceRegistry() = default;

	bool register_imu(std::unique_ptr<IImu> imu);
	bool register_gnss(std::unique_ptr<IGnss> gnss);

	bool init_all();
	bool start_all();
	bool stop_all();

private:
	HalContext ctx_;

	std::array<std::unique_ptr<IImu>, CONFIG_MAX_IMU_DEVICE> imu_instances_;
	std::array<std::unique_ptr<IGnss>, CONFIG_MAX_GNSS_DEVICE> gnss_instances_;

	uint8_t imu_count_{0};
	uint8_t gnss_count_{0};

	bool start_all_imu();
	bool start_all_gnss();
};


