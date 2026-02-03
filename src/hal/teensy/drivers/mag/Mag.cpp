#include <drivers/mag/Mag.hpp>
#include "Mag.hpp"

Mag::Mag(){}
Mag::~Mag(){}

bool Mag::init(){
    bool res = mag.init(MAG_ADDRESS, X_MAG_OFFSET, Y_MAG_OFFSET, DECLINATION);

    return res;
}

float Mag::readHeading()
{
    float heading = mag.getHeading();
    return heading;
}

Vector3f Mag::readRawValues()
{
    return {mag.getRawX(), mag.getRawY(), mag.getRawZ()};
}
