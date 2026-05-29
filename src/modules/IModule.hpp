#pragma once

class IModule {
public:
    virtual ~IModule() = default;
    virtual void init() = 0;            // Inizializzazione del modulo
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void run() = 0; 
};