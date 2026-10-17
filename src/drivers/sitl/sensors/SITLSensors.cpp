#include "SITLSensors.hpp"

SITLSensors::SITLSensors():
	sim_(CONFIG_AUTOPILOT_PORT, "127.0.0.1", CONFIG_SIM_PORT)
{
	
}

SITLSensors::~SITLSensors()
{
}
