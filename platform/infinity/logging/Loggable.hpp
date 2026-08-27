// loggable.hpp
#pragma once
#include <cstdint>
#include <cstdarg>

struct log_source_const_data;  // forward decl: evita di trascinare log.h ovunque

class Loggable {
protected:
    bool init_logging(const struct log_source_const_data* inst);

    void log_dbg(const char* fmt, ...);
    void log_inf(const char* fmt, ...);
    void log_wrn(const char* fmt, ...);
    void log_err(const char* fmt, ...);

private:
    const struct log_source_const_data* log_inst = nullptr;
};