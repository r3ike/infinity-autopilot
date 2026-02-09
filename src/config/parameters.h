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

    #define LOOP_RATE_HARD_LOOP 500         // Frequenza di esecuzione dell'hard-loop
    #define LOOP_RATE_GPS 1000              // Frequenza di interrogazione al GPS
    #define LOOP_RATE_LOGGING_SD 50         // Frequenza di logging su SD   
    #define LOOP_RATE_TELEMETRY 20          // Frequenza con cui vengono inviati messaggi di TLM


    /*-----------------------
        MOTOR CONSTRAINS [hz]
    -------------------------*/
    #define MOTOR_STOP 1000
    #define MOTOR_MIN 1000                  //Per il minimo del motore (Da calcolare).
    #define MOTOR_MAX 1800                  //Massimo del motore.

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
    #define ACC_CUTOFF_FREQ 80              // Cut-Off Frequency per l' LPF dell'accelerometro
    #define GYRO_CUTOFF_FREQ 200            // Cut-Off Frequency per l' LPF del giroscopio
    #define MAG_CUTOFF_FREQ 5               // Cut-Off Frequency per l' LPF del magnetometro

    /*-----------------------
        LOGS FLAGS
    -------------------------*/
    #define BUFFER_SIZE_KB 32               // Dimensione in KB del buffer circolare usato per salvare i log in attesa che vengano salvati su SD.
    #define LOG_IMU_FLAG true               // Flag per indicare se si vuole loggare i dati che arrivano dalla IMU.
    #define LOG_MAG_FLAG true               // Flag per indicare se si vuole loggare i dati che arrivano dal MAG.
    #define LOG_GPS_FLAG true               // Flag per indicare se si vuole loggare i dati che arrivano dal GPS.
    #define LOG_LIDAR_FLAG true             // Flag per indicare se si vuole loggare i dati che arrivano dal LIDAR.

    #define LOG_EKF_FLAG true               // Flag per indicare se si vuole loggare le metriche riguardante l'EKF.
    #define LOG_TASK_TRACE_FLAG true        // Flag per indicare se si vuole loggare le metriche riguardanti i tempi delle Task.
    #define LOG_SYS_INFO_FLAG true          // Flag per indicare se si vuole loggare le informazioni generali del sistema.
    #define LOG_CONTROL_FLAG true           // Flag per indicare se si vuole loggare i dati relativi al controller.
    #define LOG_STATE_CHANGES_FLAG true     // Flag per indicare se si vuole loggare i cambi di stato del sistema.


    /**--------------------------------
     *      MULTI-SENSORS-INSTANCES
     ---------------------------------*/
    #define IMU_INSTANCES 1
    #define BARO_INSTANCES 1
    #define MAG_INSTANCES 1
    #define GPS_INSTANCES 1
    #define LIDAR_INSTANCES 1
#endif