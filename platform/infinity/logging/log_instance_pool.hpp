// log_instance_pool.hpp
#pragma once
#include <zephyr/logging/log_instance.h>
#include <zephyr/sys/util.h>   // LISTIFY, ARRAY_SIZE

/*
 * Macro helper interne, chiamate da LISTIFY una volta per indice (0..count-1).
 * NON usarle direttamente.
 */
#define Z_LOG_POOL_REGISTER_ONE(idx, _module) \
    LOG_INSTANCE_REGISTER(_module, idx, CONFIG_LOG_DEFAULT_LEVEL)

#define Z_LOG_POOL_INIT_ONE(idx, _module) \
    { LOG_INSTANCE_PTR_INIT(log, _module, idx) }

/*
 * Genera un pool di _count sorgenti di log per la "famiglia" di moduli _module.
 *
 * IMPORTANTE: _count deve essere un LETTERALE intero noto al preprocessore
 * (es. 2, o una macro tipo DT_NUM_INST_STATUS_OKAY(...) o CONFIG_XXX),
 * MAI un'espressione tipo (A + B): LISTIFY fa token-pasting e non sa fare
 * aritmetica sui token, quindi "(1 + 1)" spacca la macro.
 *
 * Genera:
 *  - struct <_module>_log_slot
 *  - array statico <_module>_log_pool[_count]
 *  - funzione <_module>_log_instance_get(idx) -> puntatore o NULL se fuori range
 */
#define LOG_INSTANCE_POOL_DEFINE(_module, _count)                              \
    struct _module##_log_slot {                                                \
        LOG_INSTANCE_PTR_DECLARE(log);                                         \
    };                                                                         \
    LISTIFY(_count, Z_LOG_POOL_REGISTER_ONE, (;), _module);                    \
    static struct _module##_log_slot _module##_log_pool[] = {                  \
        LISTIFY(_count, Z_LOG_POOL_INIT_ONE, (,), _module)                     \
    };                                                                         \
    static inline const struct log_source_const_data*                         \
    _module##_log_instance_get(uint8_t idx) {                                  \
        return (idx < ARRAY_SIZE(_module##_log_pool))                          \
                   ? _module##_log_pool[idx].log                               \
                   : NULL;                                                     \
    }