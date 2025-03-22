#pragma once

#include "defines.h"

#define MAX_MESSAGE_SIZE 64

//TODO: functions for connecting sockets

u8 rcv_req(u32 c_id);
u8 snd_req(u32 c_id, const char* msg);
