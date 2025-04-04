#include "game.h"

#include "asserts.h"
#include "logger.h"
#include "files.h"
#include "socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static game_state* gstate = 0;

u8 load_items_from_file(const char* filepath, char* items_list);
u8 send_items_to_server(const char* items_list); 
u8 get_items_from_server(char* items_list_opp); 
u8 load_game_state(const char* items_list_player, const char* items_list_opp); 

u8 init_game(const char* mech_name) {
    SM_ASSERT(gstate == 0);
    gstate = (game_state*)malloc(sizeof(game_state));

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

    return true;
}

void free_game() {
    SM_ASSERT(gstate != 0);
    free(gstate);
    gstate = 0;
    LINFO("uninitialized game state");
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
    return true;
}

u8 load_game_state(const char* items_list_player, const char* items_list_opp) {
    //TODO this will be a lot of copying server code from game_data/item_data
    return true;
}
