#include "logger.h"

#include <stdio.h>
#include <stdarg.h>

void log_message(u32 msg_type, const char* msg, ...) {

    char output[16384];
    va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(output, 16384, msg, arg_ptr);
    va_end(arg_ptr);

    switch(msg_type) {
        case LOG_ERROR:
            printf("\e[0;31m[ERROR]: %s\e[0m\n", output);
            break;
        case LOG_WARNING:
            printf("\e[0;33m[WARN]: %s\e[0m\n", output);
            break;
        case LOG_INFO:
            printf("\e[0;32m[INFO]: %s\e[0m\n", output);
            break;
        case LOG_DEBUG:
            printf("\e[0;34m[DEBUG]: %s\e[0m\n", output);
            break;
        case LOG_TRACE:
            printf("[TRACE]: %s\n", output);
            break;
    }
}
