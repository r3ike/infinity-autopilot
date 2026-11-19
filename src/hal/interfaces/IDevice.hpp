#pragma once
#include <cstdint>
#include <string_view>

#include <zephyr/kernel.h>
/**
 * Base comune per tutte le interfacce dei vari dispostivi ()
 */

class HalContext;

enum class DeviceState : uint8_t {
    Uninitialized,
    Initializing,
    Ready,
    Failed,
    Stopped
};

class IDevice
{
public:
 	static constexpr size_t MAX_STR_LEN = CONFIG_DEVICE_NAME_MAX_LEN;
	virtual ~IDevice() = default;

	std::string_view name() const {
		return name_.empty() ? std::string_view{"<uninitialized>"} : name_;
	}
	uint8_t id() const {return instance_id_;} 

	DeviceState state() const {return state_;}
	bool is_ready() const {return state_ == DeviceState::Ready;}

	virtual bool init(uint8_t instance_id, HalContext& ctx) = 0;


protected:
	void assign_identity(uint8_t instance_id, std::string_view model){
		int ret = snprintk(name_buf_, MAX_STR_LEN, "%.*s_%u",
                           static_cast<int>(model.size()), model.data(),
                           static_cast<unsigned>(instance_id));

        if (ret < 0 || ret >= static_cast<int>(MAX_STR_LEN)) {
            name_buf_[MAX_STR_LEN - 1] = '\0';
        }
        name_ = name_buf_; 
		instance_id_ = instance_id;
	}

	bool mark_device_state(const DeviceState new_state) {
		if (!is_transition_allowed(state_, new_state))
		{
			return false;
		}
		
		state_ = new_state;
		return true;
	}

private:
	uint8_t instance_id_ = 0xFF;
	std::string_view name_{};		
	char name_buf_[MAX_STR_LEN]{}; 
    
	DeviceState state_ = DeviceState::Uninitialized;

	/**
	 * Macchina a stati per verificare che la transizione sia possibile
	 * 
	 * 	Uninitialized ──init()──▶ Initializing ──successo──▶ Ready
     *                        │
     *                        └──fallimento──▶ Failed
	 * 
	 *	Ready ──stop()──▶ Stopped
	 */
	static constexpr bool is_transition_allowed(DeviceState from, DeviceState to) {
        switch (from) {
            case DeviceState::Uninitialized:
                return to == DeviceState::Initializing
                    || to == DeviceState::Ready        // se salti Initializing
                    || to == DeviceState::Failed;
            case DeviceState::Initializing:
                return to == DeviceState::Ready
                    || to == DeviceState::Failed;
            case DeviceState::Ready:
                return to == DeviceState::Stopped;
            case DeviceState::Failed:
                return to == DeviceState::Initializing   // retry
                    || to == DeviceState::Ready;         // se retry diretto
            case DeviceState::Stopped:
                return to == DeviceState::Initializing;  // riavvio
        }
        return false;
    }
 };

 