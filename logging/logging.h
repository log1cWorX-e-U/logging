#pragma once

#include <stdarg.h>

void logging_log_message(const char* message);
void logging_log_formatted(const char* format, ...);
void logging_log_with_ID(const char* module_id, const char* function, const char* format, ...);

#if LOGGING_ENABLED == 1
    #define LOG(module_id, ...) logging_log_with_ID(module_id, __FUNCTION__, __VA_ARGS__)
#else
    #define LOG(module_id, ...) ((void)0)
#endif
