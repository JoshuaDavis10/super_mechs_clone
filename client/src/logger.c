#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

void log_message(u32 msg_type, const char* msg, ...) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    char output[16384];
    va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(output, 16384, msg, arg_ptr);
    va_end(arg_ptr);

    switch(msg_type) {
        case SM_LOG_ERROR:
            SetConsoleTextAttribute(hConsole, 4); 
            printf("[ERROR]: %s\n", output);
            SetConsoleTextAttribute(hConsole, 7); 
            break;
        case SM_LOG_WARNING:
            SetConsoleTextAttribute(hConsole, 6); 
            printf("[WARN]: %s\n", output);
            SetConsoleTextAttribute(hConsole, 7); 
            break;
        case SM_LOG_INFO:
            SetConsoleTextAttribute(hConsole, 2); 
            printf("[INFO]: %s\n", output);
            SetConsoleTextAttribute(hConsole, 7); 
            break;
        case SM_LOG_DEBUG:
            SetConsoleTextAttribute(hConsole, 1); 
            printf("[DEBUG]: %s\n", output);
            SetConsoleTextAttribute(hConsole, 7); 
            break;
        case SM_LOG_TRACE:
            printf("[TRACE]: %s\n", output);
            break;
    }
}
