
#include "../include/raylib.h"

#include "defines.h"

#include "socket.h"
#include "logger.h"
#include "asserts.h"
#include "game.h"
#include "game_data.h"
#include "item_data.h"

#include <stdio.h>
#include <string.h>

        
int main(int argc, char** argv) {

    SM_ASSERT_MSG(argc == 3, "usage: ./client server_name mech_file");
    char buffer[MAX_MESSAGE_SIZE];
    char command[MAX_MESSAGE_SIZE];

    if(!init_game_data()) {
        LERROR("failed to initialize game data. terminating program...");   
        return -1;
    }

    if(!load_item_data()) {
        LERROR("failed to load item data. terminating program...");   
        free_game_data();
        return -1;

    }

    if(!conn_to_serv(argv[1])) {
        return -1;
    }

    if(!init_game(argv[2])) {
        LERROR("failed to initialize game state. terminating program...");
        free_item_data();
        free_game_data();
        return -1;
    }

    recv_msg(buffer);
    while(strcmp(buffer, "loss") != 0 && strcmp(buffer, "win") != 0) {
        if(strcmp(buffer, "go") == 0) {
            LINFO("enter command:");
            fgets(command, sizeof(command), stdin);
            command[strcspn(command, "\n")] = 0;
            send_msg(command);
        }
        else if(strcmp(buffer, "valid") == 0) {
            LTRACE("command was valid");
            send_msg("ACK");
            process_command(command);
        }
        else if(strcmp(buffer, "invalid") == 0) {
            LWARN("command was invalid");
        }
        else {
            LTRACE("received opponent's command: %s", buffer);
            process_command(buffer);
        }
        recv_msg(buffer);
    }

/*
    LINFO("initializing raylib window...");
    InitWindow(1000, 500, "super-mechs-clone v0.0.1");
    LINFO("initialized raylib window");

    LINFO("starting game loop...");
    while(!WindowShouldClose()) { 
        BeginDrawing();
            
        ClearBackground(ORANGE);

        EndDrawing();
    }
    LINFO("closed raylib window");
    */

    //TODO: disconnect socket??? is that a thing...?
    return 0;
}
