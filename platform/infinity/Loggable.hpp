#pragma once
#include <zephyr/logging/log.h>
#include <cstdio>
#include <stdint.h>

/**
 * Classe che i moduli dovranno estendere per poter scrivere dei messaggi di log.
 * tale classe supporta anche il logging multi-instance per queli moduli/driver che hanno più
 * istanze.
 * 
 * tale classe è un wrapper del sistema di logging nativo di zephyr.
 * 
 * TODO:
 *  - valutare se direttamente all'interno dei metodi per il logging di messaggi di WARN/ERROR
 *    inviare un evento/msg per attuare azioni di contromisura per contrastare questi errori.
 */

 // da mettere come kconfig
#define MAX_LOG_MODULE_NAME_LEN 64      

class Loggable
{
protected:
    bool init_logging(const char* name, int level = LOG_LEVEL_DBG){
        
        log_inst = log_instance_ptr_init(name, level);
        
        return log_inst != nullptr;
    }

    bool init_logging(const char* name, uint8_t instance_id, int level = LOG_LEVEL_DBG){
        char complete_name[MAX_LOG_MODULE_NAME_LEN];
        snprintk(complete_name, sizeof(complete_name), "%s_%u", name, instance_id);
        log_inst = log_instance_ptr_init(complete_name, level);

        return log_inst != nullptr;
    }

    void log_dbg(const char* fmt, ...){
        va_list args;
        va_start(args, fmt);
        log_instance_msg(log_inst, LOG_LEVEL_DBG, fmt, args);
        va_end(args);
    }

    void log_inf(const char* fmt, ...){
        va_list args;
        va_start(args, fmt);
        log_instance_msg(log_inst, LOG_LEVEL_INF, fmt, args);
        va_end(args);
    }

    void log_wrn(const char* fmt, ...){
        va_list args;
        va_start(args, fmt);
        log_instance_msg(log_inst, LOG_LEVEL_WRN, fmt, args);
        va_end(args);
    }

    void log_err(const char* fmt, ...){
        va_list args;
        va_start(args, fmt);
        log_instance_msg(log_inst, LOG_LEVEL_ERR, fmt, args);
        va_end(args);
    }

private:
    const struct log_source_const_data* log_inst = nullptr;
};

