#pragma once

#include "IDevice.hpp"
#include "IStreamingDevice.hpp"

class IImu : public IDevice, public IStreamingDevice
{

public:
	virtual ~IImu() = default;

	//virtual void set_range() = 0;
	//virtual void reset() = 0;

};

