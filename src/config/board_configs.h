#pragma once
/*-----------------------
        BOARD CONFIG
-------------------------*/
#define WIRE_CLOCK 400000

#define MAG_ADDRESS 0x1E

/*-----------------------
        PIN CONFIG
-------------------------*/
#define BLUE_LED_PIN 23
#define RED_LED_PIN 15
#define GREEN_LED_PIN 14

#define MOTOR_FL_PIN 3
#define MOTOR_FR_PIN 4
#define MOTOR_BL_PIN 2
#define MOTOR_BR_PIN 5

#ifndef BUILTIN_SDCARD
#define BUILTIN_SDCARD 254
#endif

#define GPS_SERIAL Serial4
#define LIDAR_SERIAL Serial2
