#include <drivers/motor/Motor.hpp>
#include "Motor.hpp"

Motor::Motor(){}
Motor::~Motor(){}

void Motor::init()
{
    _motor_FL.attach(MOTOR_FL_PIN);
    _motor_FR.attach(MOTOR_FR_PIN);
    _motor_BL.attach(MOTOR_BL_PIN);
    _motor_BR.attach(MOTOR_BR_PIN);

    stop();
}

void Motor::stop(){
    _motor_FL.writeMicroseconds(MOTOR_STOP);
    _motor_FR.writeMicroseconds(MOTOR_STOP);
    _motor_BL.writeMicroseconds(MOTOR_STOP);
    _motor_BR.writeMicroseconds(MOTOR_STOP);
}

void Motor::write(int fl, int fr, int bl, int br){
    
    _motor_FL.writeMicroseconds(std::clamp(fl, MOTOR_MIN, MOTOR_MAX));
    _motor_FR.writeMicroseconds(std::clamp(fr, MOTOR_MIN, MOTOR_MAX));
    _motor_BL.writeMicroseconds(std::clamp(bl, MOTOR_MIN, MOTOR_MAX));
    _motor_BR.writeMicroseconds(std::clamp(br, MOTOR_MIN, MOTOR_MAX));
}
