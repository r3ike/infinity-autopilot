#pragma once

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/autoconf.h>

#include <cstring>
#include <unistd.h>       
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "HAL.hpp"

class Radio_sitl : public IHAL_Radio
{
private:
    int _sock;
    struct sockaddr_in _addr;           // Mittente (firmware)
    struct sockaddr_in _gcs_addr;       // Ground control station socket
public:
    Radio_sitl();
    ~Radio_sitl() = default;

    bool init() override;
    void send_to_GCS(uint8_t* buffer, size_t len) override;
    long recive_from_GCS(uint8_t* buffer, size_t len) override;
};



