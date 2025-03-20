#pragma once

#include "defines.h"

//TODO: functions for connecting sockets

u8 rcv_req(u32 c_id);
u8 snd_req(u32 c_id, const char* msg);
