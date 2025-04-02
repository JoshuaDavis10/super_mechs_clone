#pragma once

#include "defines.h"

u32 conn_to_serv(const char* hostname); 
u32 recv_msg(char* msg); 
u32 send_msg(const char* msg);
