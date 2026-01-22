#include <./hal/teensy/lib/HMC5883l/HMC5883l.h>


HMC5883l::HMC5883l(){}
HMC5883l::~HMC5883l(){}

bool HMC5883l::init(){
    _address = 0x1E;
    _x_offset = 0.0f;
    _y_offset = 0.0f;
    _declination = 0.0f;

    //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(_address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //continuous measurement mode
    Wire.endTransmission();

    return true;
}

bool HMC5883l::init(int address, float xOffset, float yOffset, float declination){
    _address = address;
    _x_offset = xOffset;
    _y_offset = yOffset;
    _declination = declination;

    //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(_address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //continuous measurement mode
    Wire.endTransmission();

    return true;
}

float HMC5883l::getHeading(){
    int x,y,z; //triple axis data
    //Tell the HMC5883 where to begin reading data
    Wire.beginTransmission(_address);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();
    
    
    //Read data from each axis, 2 registers per axis
    Wire.requestFrom(_address, 6);
    if(6<=Wire.available()){
        x = Wire.read()<<8; //X msb
        x |= Wire.read(); //X lsb
        z = Wire.read()<<8; //Z msb
        z |= Wire.read(); //Z lsb
        y = Wire.read()<<8; //Y msb
        y |= Wire.read(); //Y lsb
    }

    // Applica gli offset di calibrazione
    float xCalib = x - _x_offset;
    float yCalib = y - _y_offset;

    // Calcola l'heading in radianti
    float heading = atan2(yCalib, xCalib);

    // Converti l'heading da radianti a gradi
    heading = heading * 180.0 / M_PI;

    // Aggiungi la declinazione magnetica
    heading += _declination;

    // Assicurati che l'heading sia tra 0 e 360 gradi
    if (heading < 0) {
        heading += 360.0;
    } else if (heading > 360) {
        heading -= 360.0;
    }

    heading = (heading * M_PI) / 180;

    return heading;
}

 int HMC5883l::getRawX(){
    int x,y,z; //triple axis data
    //Tell the HMC5883 where to begin reading data
    Wire.beginTransmission(_address);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();
    
    
    //Read data from each axis, 2 registers per axis
    Wire.requestFrom(_address, 6);
    if(6<=Wire.available()){
        x = Wire.read()<<8; //X msb
        x |= Wire.read(); //X lsb
        z = Wire.read()<<8; //Z msb
        z |= Wire.read(); //Z lsb
        y = Wire.read()<<8; //Y msb
        y |= Wire.read(); //Y lsb
    }

    // Applica gli offset di calibrazione
    float xCalib = x - _x_offset;

    return xCalib;
 }

 int HMC5883l::getRawY(){
    int x,y,z; //triple axis data
    //Tell the HMC5883 where to begin reading data
    Wire.beginTransmission(_address);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();
    
    
    //Read data from each axis, 2 registers per axis
    Wire.requestFrom(_address, 6);
    if(6<=Wire.available()){
        x = Wire.read()<<8; //X msb
        x |= Wire.read(); //X lsb
        z = Wire.read()<<8; //Z msb
        z |= Wire.read(); //Z lsb
        y = Wire.read()<<8; //Y msb
        y |= Wire.read(); //Y lsb
    }

    // Applica gli offset di calibrazione
    float yCalib = y - _y_offset;

    return yCalib;
 }

 int HMC5883l::getRawZ(){
    int x,y,z; //triple axis data
    //Tell the HMC5883 where to begin reading data
    Wire.beginTransmission(_address);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();
    
    
    //Read data from each axis, 2 registers per axis
    Wire.requestFrom(_address, 6);
    if(6<=Wire.available()){
        x = Wire.read()<<8; //X msb
        x |= Wire.read(); //X lsb
        z = Wire.read()<<8; //Z msb
        z |= Wire.read(); //Z lsb
        y = Wire.read()<<8; //Y msb
        y |= Wire.read(); //Y lsb
    }


    return z;
 }
