#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/autoconf.h>

#include "udp_socket/UdpSocket.hpp"

#include <zephyr/kernel.h>


class SITLSensors
{

public:
	SITLSensors();
	~SITLSensors();

private:
	UdpSocket sim_{0};
};



