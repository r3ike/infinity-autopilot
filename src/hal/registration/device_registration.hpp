// hal/registration/hal_driver_registration.hpp
#pragma once
#include <zephyr/sys/iterable_sections.h>

class DeviceRegistry;
struct HalContext;

// Il "record" che ogni driver deposita nella sezione
struct HalDriverRegistrar {
    // Nome del compatible (solo per debug/log, opzionale)
    const char* compatible;

    // Funzione che registra le istanze di quel driver nel registry
    void (*register_fn)(DeviceRegistry& registry);
};

// Macro che ogni file driver usa UNA VOLTA per auto-registrarsi
#define HAL_REGISTER_ADAPTER(compat_str, fn_ptr) \
    STRUCT_SECTION_ITERABLE(HalDriverRegistrar, \
        _hal_registrar_##fn_ptr) = { \
            .compatible = (compat_str), \
            .register_fn = (fn_ptr), \
        }
