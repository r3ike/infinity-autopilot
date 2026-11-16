#pragma once

#include "IDevice.hpp"

class IImu : public IDevice
{

public:
	virtual ~IImu() = default;

	virtual void set_range() = 0;
	virtual void reset() = 0;

};

