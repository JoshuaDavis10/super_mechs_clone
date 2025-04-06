
#include "../include/raylib.h"

#include "defines.h"

#include "socket.h"
#include "logger.h"
#include "asserts.h"
#include "game_data.h"
#include "item_data.h"

#include <stdio.h>
#include <string.h>

        
int main(int argc, char** argv) {

    SM_ASSERT_MSG(argc == 3, "usage: ./client server_name mech_file");
    char buffer[MAX_MESSAGE_SIZE];
    char command[MAX_MESSAGE_SIZE];
    i32 res;
    u8 input_flag = false;
    i32 pos[2];

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

    LINFO("initializing raylib window...");
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "super-mechs-clone v0.0.1");
    LINFO("initialized raylib window");

    LINFO("starting game loop...");

    while(!WindowShouldClose()) { 
        res = check_for_msg(); 
        SM_ASSERT(res != -1);
        if(res == true) {
            recv_msg(buffer);
            if(strcmp(buffer, "win") == 0) {
                LINFO("you won!");
                CloseWindow();
                break;
            }

            else if(strcmp(buffer, "loss") == 0) {
                LINFO("you lost.");
                CloseWindow();
                break;
            }

            else if(strcmp(buffer, "go") == 0) {
                input_flag = true;
                LTRACE("input flag set to true");
            }
            else if(strcmp(buffer, "invalid") == 0) {
                LWARN("command was invalid");
            }
            else {
                LTRACE("received opponent's command: %s", buffer);
                process_command(buffer);
            }
        }

        if(input_flag) {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                send_msg("l1");
                recv_msg(buffer);
                if(strcmp(buffer, "valid") == 0) {
                    LTRACE("command was valid");
                    send_msg("ACK");
                    process_command("l1");
                }
                input_flag = false;
                LTRACE("input flag set to false");
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                send_msg("r1");
                recv_msg(buffer);
                if(strcmp(buffer, "valid") == 0) {
                    LTRACE("command was valid");
                    send_msg("ACK");
                    process_command("r1");
                }
                input_flag = false;
                LTRACE("input flag set to false");
            }
        }

        pos[P1] = get_pos(P1);
        pos[P2] = get_pos(P2);

        BeginDrawing();

        ClearBackground(ORANGE);
        DrawRectangle(WINDOW_WIDTH/10*pos[P1], 4*WINDOW_HEIGHT/5, WINDOW_WIDTH/10, WINDOW_HEIGHT/5, GREEN);
        DrawRectangle(WINDOW_WIDTH/10*pos[P2], 4*WINDOW_HEIGHT/5, WINDOW_WIDTH/10, WINDOW_HEIGHT/5, RED);

        EndDrawing();
    }
    LINFO("closed raylib window");

    //TODO: disconnect socket???
    return 0;
}
