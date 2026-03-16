#include "FlightController.hpp"

static FlightController fc;

int main()
{
    fc.init();

    fc.start();
    return 0;
}