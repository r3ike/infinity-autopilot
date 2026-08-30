#pragma once
#include <cstdint>

class Integrator
{
public:
    Integrator();
    ~Integrator();

private:
    float total_integral_dt_ {0};
    uint64_t integrated_samples_ {0};

    float min_reset_dt_ {0};
    uint64_t min_reset_samples {0};
};

