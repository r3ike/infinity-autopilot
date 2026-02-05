#pragma once 
#include <config/parameters.h>
#include <utils/Convertions.hpp>
#include <utils/uav_data.hpp>
#include <modules/Logger/Logger.hpp>

#include <hal/HAL.hpp>
#include <filters/ImuLpfFilter.hpp>

class FlightController
{
private:
    HAL hal;

    ImuLpfFilter _imu_lpf_filter;
    
    FlightController(/* args */);
    ~FlightController();

    void hardLoop();

public:

    // Metodo statico per ottenere l'unica istanza (il cuore del Singleton)
    static FlightController& getInstance();

    // Elimina il copy constructor e l'operatore di assegnazione
    // (Per evitare che qualcuno cloni il singleton per sbaglio)
    FlightController(const FlightController&) = delete;
    void operator=(const FlightController&) = delete;
    
    void init();

    // Wrapper statico per il timer
    static void hardLoopWrapper();
    
    void runSoftLoop();

};


