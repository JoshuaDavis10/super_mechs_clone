#pragma once

#include "../defines.h"

#define LOGGER_ENABLE_WARNING 1
#define LOGGER_ENABLE_INFO 1
#define LOGGER_ENABLE_DEBUG 1
#define LOGGER_ENABLE_TRACE 1 
#define LOGGER_ENABLE_NET 1 
#define LOGGER_ENABLE_UI 1 
#define LOGGER_ENABLE_ANIM 1 

enum {
    SM_LOG_ERROR = 0,
    SM_LOG_WARNING = 1,
    SM_LOG_INFO = 2,
    SM_LOG_DEBUG = 3,
    SM_LOG_TRACE = 4,
    SM_LOG_NET = 5,
    SM_LOG_UI = 6,
    SM_LOG_ANIM = 7
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

#if LOGGER_ENABLE_TRACE == 1
#define LTRACE(message, ...); log_message(SM_LOG_TRACE, message, ##__VA_ARGS__)
#else
#define LTRACE(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_NET == 1
#define LNET(message, ...); log_message(SM_LOG_NET, message, ##__VA_ARGS__)
#else
#define LNET(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_NET == 1
#define LUI(message, ...); log_message(SM_LOG_UI, message, ##__VA_ARGS__)
#else
#define LUI(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_ANIM == 1
#define LANIM(message, ...); log_message(SM_LOG_ANIM, message, ##__VA_ARGS__)
#else
#define LANIM(message, ...) //do nothing
#endif
