#pragma once 
#include <config/parameters.h>
#include <utils/Convertions.hpp>
#include <hal/HAL.hpp>
#include <utils/uav_data.hpp>


class FlightController
{
private:
    HAL hal;
    
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


