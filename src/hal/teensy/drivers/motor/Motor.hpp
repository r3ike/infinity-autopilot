#pragma once

#include <Servo.h>
#include <Arduino.h>
#include <config/board_configs.h>
#include <config/parameters.h>

class Motor
{
private:
    Servo _motor_FR;
    Servo _motor_FL;
    Servo _motor_BR;
    Servo _motor_BL;

public:
    Motor();
    ~Motor();

    void init();
    void write(int fl, int fr, int bl, int br);
    void stop();
    
};


