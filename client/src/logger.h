#pragma once

#include "defines.h"

#define LOGGER_ENABLE_WARNING 1
#define LOGGER_ENABLE_INFO 1
#define LOGGER_ENABLE_DEBUG 1
#define LOGGER_ENABLE_TRACE 1 

enum {
    SM_LOG_ERROR = 0,
    SM_LOG_WARNING = 1,
    SM_LOG_INFO = 2,
    SM_LOG_DEBUG = 3,
    SM_LOG_TRACE = 4
};

void log_message(u32 msg_type, const char* msg, ...);

#define LERROR(message, ...); log_message(SM_LOG_ERROR, message, ##__VA_ARGS__)

#if LOGGER_ENABLE_WARNING == 1
#define LWARN(message, ...); log_message(SM_LOG_WARNING, message, ##__VA_ARGS__)
#else
#define LWARN(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_INFO == 1
#define LINFO(message, ...); log_message(SM_LOG_INFO, message, ##__VA_ARGS__)
#else
#define LINFO(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_DEBUG== 1
#define LDEBUG(message, ...); log_message(SM_LOG_DEBUG, message, ##__VA_ARGS__)
#else
#define LDEBUG(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_TRACE== 1
#define LTRACE(message, ...); log_message(SM_LOG_TRACE, message, ##__VA_ARGS__)
#else
#define LTRACE(message, ...) //do nothing
#endif
