#pragma once

/**
 * CLASS: IComponent
 * DESC: This class is the base of all the component, it contains only what is needed
 * 		 by all the components
 */

class HalContext;

enum class DeviceState : uint8_t {
    Uninitialized,
    Initializing,
    Ready,      // configurato, non streamma
    Started,    // streaming attivo  ← nuovo
    Failed,
    Stopped     // shutdown definitivo (opzionale, vedi §6)
};

class IComponent
{

public:
	virtual ~IComponent() = default;

	std::string_view name() const {
		return name_.empty() ? std::string_view{"<uninitialized>"} : name_;
	}
	uint8_t id() const {return instance_id_;} 

	DeviceState state() const {return state_;}
	bool is_ready() const {return state_ == DeviceState::Ready;}
	bool is_started() const {return state_ == DeviceState::Started;}

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
	char name_buf_[CONFIG_DEVICE_NAME_MAX_LEN]{}; 
    
	DeviceState state_ = DeviceState::Uninitialized;

	/**
	 * 
	 * Uninitialized ──► Initializing ──► Ready ─────────────────► Started
	 *                       │              ▲  ▲                       │
	 *                       │ fallita      │  └────── stop() ─────────┘
	 *                       ▼              │
	 *                    Failed ─── retry ─┘
	 *                       │
	 *                       └─► Initializing  (retry)
	 * 
	 *                   Ready ──► Stopped  (shutdown definitivo, opzionale)
	 */
	static constexpr bool is_transition_allowed(DeviceState from, DeviceState to) {
        switch (from) {
			case DeviceState::Uninitialized:
				return to == DeviceState::Initializing
					|| to == DeviceState::Ready        // fast-path
					|| to == DeviceState::Failed;
			case DeviceState::Initializing:
				return to == DeviceState::Ready
					|| to == DeviceState::Failed;
			case DeviceState::Ready:
				return to == DeviceState::Started      // start()
					|| to == DeviceState::Failed
					|| to == DeviceState::Stopped;     // shutdown
			case DeviceState::Started:
				return to == DeviceState::Ready        // stop()
					|| to == DeviceState::Failed;
			case DeviceState::Failed:
				return to == DeviceState::Initializing; // retry
			case DeviceState::Stopped:
				return to == DeviceState::Initializing; // riavvio
		}
		return false;
    }
};

