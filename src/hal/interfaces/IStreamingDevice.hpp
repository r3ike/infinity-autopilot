#pragma once
#include "IComponent.hpp"

class IStreamingDevice : virtual public IComponent
{
public:
	virtual bool start() = 0;
	virtual bool stop() = 0;
};

