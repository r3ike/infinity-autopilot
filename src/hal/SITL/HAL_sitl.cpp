#include <hal/SITL/HAL_sitl.hpp>
#include "HAL_sitl.hpp"

HAL_IMU_SITL::HAL_IMU_SITL() {}

bool HAL_IMU_SITL::init()
{
    return false;
}

void HAL_IMU_SITL::calib()
{
}

Vector3f HAL_IMU_SITL::readGyro()
{
    return {0, 0, 0};
}

Vector3f HAL_IMU_SITL::readAccel()
{
    return {0, 0, 0};
}

ImuData HAL_IMU_SITL::read()
{
    return ImuData();
}

HAL_MOTOR_SITL::HAL_MOTOR_SITL()
{
}

void HAL_MOTOR_SITL::write(float m1, float m2, float m3, float m4)
{
}

HAL_TIME_INTERRUPS_SITL::HAL_TIME_INTERRUPS_SITL(/* args */) {}

unsigned long long HAL_TIME_INTERRUPS_SITL::micros()
{
    using namespace std::chrono;
    auto now = high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return (uint32_t)duration_cast<microseconds>(duration).count();
}

void HAL_TIME_INTERRUPS_SITL::startHardLoop(uint32_t frequency, void (*callback)())
{
    // Creiamo un thread che simula il Timer Interrupt
    _workerThread = std::thread([=]()
                                {
            using namespace std::chrono;
            auto next_wake = high_resolution_clock::now();
            auto period = microseconds((int)(1000000.0 / frequency));

            while(_running) {
                // Chiama il codice di volo "Hard Loop"
                callback(); 

                // Aspetta il prossimo tick preciso
                next_wake += period;
                std::this_thread::sleep_until(next_wake);
            } });
    _workerThread.detach(); // Lascialo correre in background
}

/* DELAY
void delay(uint32_t ms) override {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

*/
