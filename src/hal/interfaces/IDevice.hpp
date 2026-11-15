#pragma once

#include <cstdint>

/**
 * Base comune per tutte le interfacce dei vari dispostivi ()
 */

 class IDevice
 {
 public:
	virtual ~IDevice() = default;

	virtual bool init() = 0;

protected:
	void assign_identity(uint8_t instance_id, const char* model){

	}

private:
	uint8_t id_ = 0xFF;
    char name_buf_[32]{};
    std::string_view name_{};
    bool initialized_ = false;
 };

 