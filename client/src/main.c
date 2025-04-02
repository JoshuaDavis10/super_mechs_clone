
#include "../include/raylib.h"

#include "defines.h"

#include "socket.h"
#include "logger.h"
#include "asserts.h"
#include "game.h"

#include <stdio.h>

        
int main(int argc, char** argv) {

    SM_ASSERT_MSG(argc == 3, "usage: ./client server_name mech_file");
    char buffer[MAX_MESSAGE_SIZE];

    if(!conn_to_serv(argv[1])) {
        return -1;
    }

    if(!init_game(argv[2])) {
        LERROR("failed to initialize game state. terminating program...");
        return -1;
    }

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

    free_game();
    //TODO: disconnect socket??? is that a thing...?
    return 0;
}
