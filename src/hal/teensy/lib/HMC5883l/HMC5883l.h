#ifndef HMC5883L_H  // Controlla se QUATERNIONSLIB_H non è già stato definito
#define HMC5883L_H  // Definisce QUATERNIONSLIB_H

#include <Wire.h>

//#define address 0x1E //0011110b, I2C 7bit address of HMC5883


bool HMC5883l_initialization();

float HMC5883l_getHeading();

class HMC5883l
{
private:
    int _address;
    float _x_offset;
    float _y_offset;
    float _declination;

public:
    HMC5883l();
    ~HMC5883l();

    bool init();
    bool init(int address, float xOffset, float yOffset, float declination);
    float getHeading();

    int getRawX();
    int getRawY();
    int getRawZ();
};



#endif