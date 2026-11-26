#pragma once
#include "IDevice.hpp"
#include "IStreamingDevice.hpp"

class IGnss : public IDevice, public IStreamingDevice
{
public:
	virtual ~IGnss() = default;
};


