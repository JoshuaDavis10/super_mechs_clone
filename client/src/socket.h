#pragma once

#include "defines.h"

#define MAX_MESSAGE_SIZE 1000

u32 conn_to_serv(const char* hostname); 
u32 recv_msg(char* msg); 
u32 send_msg(const char* msg);
i8 check_for_msg();
