#include <hal/teensy/drivers/gps/Bn280_driver/Bn280_driver.hpp>
#include "Bn280_driver.hpp"

Bn280_driver::Bn280_driver() {}
Bn280_driver::~Bn280_driver() {}

bool Bn280_driver::init(Stream *serial)
{
    this->_serial_ptr = serial;

    // --- UBX CONFIG ---
    _send_UBX(UBX_CFG_RATE_10HZ, sizeof(UBX_CFG_RATE_10HZ));

    _send_UBX(UBX_DISABLE_GLL, sizeof(UBX_DISABLE_GLL));
    _send_UBX(UBX_DISABLE_GSA, sizeof(UBX_DISABLE_GSA));
    _send_UBX(UBX_DISABLE_GSV, sizeof(UBX_DISABLE_GSV));
    _send_UBX(UBX_DISABLE_VTG, sizeof(UBX_DISABLE_VTG));

    return true;
}

GpsData Bn280_driver::read()
{
    while (_serial_ptr->available())
    {
        _gps.encode(_serial_ptr->read());
    }

    if (_gps.location.isUpdated())
    {
        return {
            _gps.location.lat(),
            _gps.location.lng(),
            _gps.altitude.meters(),
            _gps.course.deg(),
            _gps.speed.mps(),
            0,0,0,
            0,0,0,
            _gps.satellites.value(),
            _gps.hdop.hdop(),
            {
                (uint8_t)_gps.date.day(),
                (uint8_t)_gps.date.month(),
                (uint8_t)(_gps.date.year() - 2000), // store 2-digit year if desired
                (uint8_t)_gps.time.hour(),
                (uint8_t)_gps.time.minute(),
                (uint8_t)_gps.time.second(),
                (uint8_t)_gps.time.centisecond()
            },
            _gps.location.age()
        };
    }
}

/*-------------------
    Private method
--------------------*/
void Bn280_driver::_send_UBX(const uint8_t *msg, uint8_t len)
{
    _serial_ptr->write(msg, len);
    _serial_ptr->flush();
}
