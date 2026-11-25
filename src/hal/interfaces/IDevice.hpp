#pragma once
#include "IComponent.hpp"

/**
 * CLASS: IDevice
 * DESC: This class need to be inherited only by the real device,
 * 		 so only by the devices that needs to be registered in the device registry
 */

class IDevice : public virtual IComponent {
public:
    // No method
};
