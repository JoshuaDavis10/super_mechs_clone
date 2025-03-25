#pragma once

#include "defines.h"

#define LOGGER_ENABLE_WARNING 1
#define LOGGER_ENABLE_INFO 1
#define LOGGER_ENABLE_DEBUG 1
#define LOGGER_ENABLE_TRACE 1 

enum {
    LOG_ERROR = 0,
    LOG_WARNING = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3,
    LOG_TRACE = 4
};

void log_message(u32 msg_type, const char* msg, ...);

#define LERROR(message, ...); log_message(LOG_ERROR, message, ##__VA_ARGS__)

#if LOGGER_ENABLE_WARNING == 1
#define LWARN(message, ...); log_message(LOG_WARNING, message, ##__VA_ARGS__)
#else
#define LWARN(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_INFO == 1
#define LINFO(message, ...); log_message(LOG_INFO, message, ##__VA_ARGS__)
#else
#define LINFO(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_DEBUG== 1
#define LDEBUG(message, ...); log_message(LOG_DEBUG, message, ##__VA_ARGS__)
#else
#define LDEBUG(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_TRACE== 1
#define LTRACE(message, ...); log_message(LOG_TRACE, message, ##__VA_ARGS__)
#else
#define LTRACE(message, ...) //do nothing
#endif
