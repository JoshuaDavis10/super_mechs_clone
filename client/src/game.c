#include "game.h"

#include "asserts.h"
#include "logger.h"
#include "files.h"
#include "socket.h"
#include "utils.h"
#include "game_data.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

u8 load_items_from_file(const char* filepath, char* items_list);
u8 send_items_to_server(const char* items_list); 
u8 get_items_from_server(char* items_list_opp); 
u8 load_game_state(char* items_list_player, char* items_list_opp); 

u8 init_game(const char* mech_name) {
    char items_buffer_player[MAX_MESSAGE_SIZE]; //TODO: 22 cringe constant
    char items_buffer_opp[MAX_MESSAGE_SIZE]; 

    if(!load_items_from_file(mech_name, items_buffer_player)) {
        LERROR("failed to read items from mech file");
        return false;
    }
    LTRACE("loaded items from mech file");

    if(!send_items_to_server(items_buffer_player)) {
        LERROR("failed to send items to server");
        return false;
    }
    LTRACE("sent player's items to server");


    if(!get_items_from_server(items_buffer_opp)) {
        LERROR("failed to get items from server");
        return false;
    }
    LTRACE("got opponent's items from server");

    if(!load_game_state(items_buffer_player, items_buffer_opp)) {
        LERROR("failed to load game state with item data");
        return false;
    }
    LTRACE("loaded game state with item data");

    LINFO("initialized game state");

    send_msg("start");

    return true;
}

u8 load_items_from_file(const char* mech_name, char* items_list) {
    file mechfile;
    char filepath[1000];
    snprintf(filepath, 1000, "mechfiles/%s.txt", mech_name);
    if(!open_file(filepath, &mechfile, "r")) {
        LERROR("failed to open file: %s", filepath);
        return false;
    }

    if(!read_line(mechfile, items_list, MAX_MESSAGE_SIZE * 22)) { //TODO: cringe constant   
        LERROR("failed to read line from file");
        return false;
    }

    LTRACE("read line: %s from mechfile", items_list);

    return true; 
}

u8 send_items_to_server(const char* items_list) {
    char msg_buffer[MAX_MESSAGE_SIZE];
    if(!send_msg(items_list)) {
        LERROR("failed to send message to server");
        return false;
    }
    if(!recv_msg(msg_buffer)) {
        LERROR("failed to receive message from server");
        return false;
    }
    if(strcmp(msg_buffer, "success") != 0) {
        LERROR("server message: %s", msg_buffer);
        return false;
    }

    return true;
}

u8 get_items_from_server(char* items_list_opp) {
    if(!recv_msg(items_list_opp)) {
        return false;
    }
    send_msg("ACK");
    return true;
}

u8 load_game_state(char* items_list_player, char* items_list_opp) {
    char pos_buffer[MAX_MESSAGE_SIZE];
    char turn_buffer[MAX_MESSAGE_SIZE];
    if(!recv_msg(pos_buffer)) { return false; }

    char *pos_str, *tok, *pos, *saveptr1, *saveptr2;
    i32 pos1, pos2;
    i32 turn;

    pos_str = strtok_r(pos_buffer, ",", &saveptr1);
    tok = strtok_r(pos_str, ":", &saveptr2);
    pos = strtok_r(NULL, ":", &saveptr2);
    SM_ASSERT(is_number(pos));
    pos1 = atoi(pos);

    pos_str = strtok_r(NULL, ",", &saveptr1);
    tok = strtok_r(pos_str, ":", &saveptr2);
    pos = strtok_r(NULL, ":", &saveptr2);
    SM_ASSERT(is_number(pos));
    pos2 = atoi(pos);

    send_msg("ACK");
    if(!recv_msg(turn_buffer)) { return false; }
    SM_ASSERT(is_number(turn_buffer));
    turn = atoi(turn_buffer);
    SM_ASSERT(turn == 0 || turn == 1);

    if(!load_game_data(items_list_player, items_list_opp, pos1, pos2, turn)) {
        return false;
    }
    return true;
}
