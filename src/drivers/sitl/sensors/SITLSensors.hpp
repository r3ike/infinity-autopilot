#pragma once

#include <cstdint>

#include "Loggable.hpp"
#include "udp_socket/UdpSocket.hpp"
#include "mavlink/common/mavlink.h"

#include "ITask.hpp"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/autoconf.h>

using namespace infinity_autopilot;


class SITLSensors : public scheduler::ITask, public Loggable
{

public:
	static constexpr scheduler::TaskConfig taskConf {
        .name = "sitl-sensors-driver",
        .priority = 0,
        .policy = scheduler::SchedPolicy::RoundRobin,
        .period_us = 0,
        .stack_size = 2048
    };

	bool init() override;

    void run() override;

	SITLSensors();
	~SITLSensors();

private:
	UdpSocket sim_socket_;

	uint64_t last_heartbeat_{0};
	uint64_t last_sim_time_{0};

	void send_mavlink_msg(mavlink_message_t& msg);
	void handle_mavlink_msg(mavlink_message_t& msg);

	void handle_hil_sensor(mavlink_hil_sensor_t& sensors_msg);
	void handle_hil_gps();

};



