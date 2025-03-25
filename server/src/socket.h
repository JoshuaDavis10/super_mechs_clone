#pragma once

#include "defines.h"

#define MAX_MESSAGE_SIZE 64

u8 init_sock_conn(); 
//TODO: close socket
void free_socket();
u8 recv_msg(char* msg, i32 c_id);
u8 send_msg(const char* msg, i32 c_id);
