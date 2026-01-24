#ifndef PARAMETERS_H
#define PARAMETERS_H

    #define ENVIRONMENT "REAL"      //REAL SITL BENCH HIL
    /*-----------------------
        PHISICAL CONSTANTS
    -------------------------*/
    #define G 9.81f
    #define MASS_DRONE 1.5f  // massa in kg
    #define DEG_TO_RAD 0.01745329252f
    #define RAD_TO_DEG 57.29577951f
    #define DECLINATION 3.833
    
    /*-----------------------
            LOOP RATES [hz]
    -------------------------*/
    #define NOMINAL_TICK_RATE 1000

    #define LOOP_RATE_HARD_LOOP 500
    #define LOOP_RATE_GPS 1000
    #define LOOP_RATE_LOGGING 50
    #define LOOP_RATE_TELEMETRY 20


    /*-----------------------
        MOTOR CONSTRAINS [hz]
    -------------------------*/
    #define MOTOR_STOP 1000
    #define MOTOR_MIN 1000              //Per il minimo del motore (Da calcolare)
    #define MOTOR_MAX 1800             //Massimo del motore

    /*-----------------------
        CALIBRATION PARAMETERS
    -------------------------*/
    #define IMU_CALIBRATION_STEP 2000
    #define LIDAR_CALIBRATION_STEP 2000

    #define X_MAG_OFFSET 11.0
    #define Y_MAG_OFFSET -78.5

    /*-----------------------
        LPF PARAMETERS
    -------------------------*/
    #define ACC_CUTOFF_FREQ 80
    #define GYRO_CUTOFF_FREQ 200
    #define MAG_CUTOFF_FREQ 5

    /*-----------------------
        LOGS FLAGS
    -------------------------*/
    #define BUFFER_SIZE_KB 32
    #define LOG_IMU_FLAG true
    #define LOG_EKF_FLAG true
    #define LOG_TASK_TRACE_FLAG true
    #define LOG_SYS_INFO_FLAG true
    #define LOG_CONTROL_FLAG true
    #define LOG_STATE_CHANGES_FLAG true

#endif