#include <drivers/gps/Gps.hpp>
#include "Gps.hpp"

Gps::Gps() {}
Gps::~Gps() {}

bool Gps::init(Stream *serial)
{
    this->serialPtr = serial;

    // --- UBX CONFIG ---
    sendUBX(UBX_CFG_RATE_10HZ, sizeof(UBX_CFG_RATE_10HZ));

    sendUBX(UBX_DISABLE_GLL, sizeof(UBX_DISABLE_GLL));
    sendUBX(UBX_DISABLE_GSA, sizeof(UBX_DISABLE_GSA));
    sendUBX(UBX_DISABLE_GSV, sizeof(UBX_DISABLE_GSV));
    sendUBX(UBX_DISABLE_VTG, sizeof(UBX_DISABLE_VTG));
}

GpsData Gps::read()
{
    while (serialPtr->available())
    {
        gps.encode(serialPtr->read());
    }

    if (gps.location.isUpdated())
    {
        return {
            gps.location.lat(),
            gps.location.lng(),
            gps.altitude.meters(),
            gps.course.deg(),
            gps.speed.mps(),
            gps.satellites.value(),
            gps.hdop.hdop(),
            {
                (uint8_t)gps.date.day(),
                (uint8_t)gps.date.month(),
                (uint8_t)(gps.date.year() - 2000), // store 2-digit year if desired
                (uint8_t)gps.time.hour(),
                (uint8_t)gps.time.minute(),
                (uint8_t)gps.time.second(),
                (uint8_t)gps.time.centisecond()
            },
            gps.location.age()
        };
    }
}

/*-------------------
    Private method
--------------------*/
void Gps::sendUBX(const uint8_t *msg, uint8_t len)
{
    serialPtr->write(msg, len);
    serialPtr->flush();
}
