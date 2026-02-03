#pragma once
#include <utils/Vector3f.h>

#include <./hal/teensy/lib/HMC5883l/HMC5883l.h>

#include <config/parameters.h>
#include <config/board_configs.h>

class Mag
{
private:
    HMC5883l mag;
public:
    Mag();
    ~Mag();

    bool init();

    float readHeading();

    Vector3f readRawValues();
};