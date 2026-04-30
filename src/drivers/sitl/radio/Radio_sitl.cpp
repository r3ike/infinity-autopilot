#include "Radio_sitl.hpp"

Radio_sitl::Radio_sitl()
{
}

bool Radio_sitl::init(){
    _sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (_sock < 0) {
        //LOG_ERR("socket() fallita: %d", sock);
        return false;
    }

    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(0);  // porta effimera
    _addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_sock, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
        //LOG_ERR("bind() fallita");
        close(_sock);
        return false;
    }

    // Ground control station destination
    memset(&_gcs_addr, 0, sizeof(_gcs_addr));
    _gcs_addr.sin_family = AF_INET;
    _gcs_addr.sin_port = htons(14550);  // QGC default
    inet_pton(AF_INET, "127.0.0.1", &_gcs_addr.sin_addr);
}

long Radio_sitl::recive_from_GCS(uint8_t *buffer, size_t len)
{
    return 0;
}

void Radio_sitl::send_to_GCS(uint8_t *buffer, size_t len)
{
}
