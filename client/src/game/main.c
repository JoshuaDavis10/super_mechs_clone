
#include "../../include/raylib.h"

#include "../defines.h"

#include "../tools/logger.h"
#include "../tools/asserts.h"
#include "../tools/event.h"
#include "network.h"
#include "graphics.h"
#include "ui.h"

#include <stdio.h>
#include <string.h>

u8 on_click_one(i32 event_type, void* listener, void* sender, event_data data) {
    LINFO("mouse clicked one! data: %d, %d", data.data[0], data.data[1]);
}

u8 on_click_two(i32 event_type, void* listener, void* sender, event_data data) {
    LINFO("mouse clicked two! data: %d, %d", data.data[0], data.data[1]);
}
        
int main(int argc, char** argv) {

    SM_ASSERT_MSG(argc == 3, "usage: ./client server_name mech_file");

    if(!startup_events()) {
        LERROR("failed to startup events.");
        return -1;
    }

    if(!startup_network(argv[1])) {
        LERROR("failed to startup network.");
        LERROR("terminating program...");
        return -1;
    }   

    if(!startup_ui(argv[2])) {
        LERROR("failed to startup ui.");
        LERROR("terminating program...");
        return -1;
    }   

    if(!startup_graphics()) {
        LERROR("failed to startup graphics system.");
        LERROR("terminating program...");
        return -1;
    }   

    LINFO("initializing raylib window...");
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "super-mechs-clone v0.0.1");
    SetTargetFPS(60);
    LINFO("initialized raylib window");

    LINFO("starting game loop...");

    u8 result;
    while(!WindowShouldClose()) { 

        result = update_network();
        if(result == 2) { CloseWindow(); break; }
        if(result == 3) { CloseWindow(); break; }
        update_graphics();
        update_ui();

        BeginDrawing();

        ClearBackground(BLACK);

        draw_graphics();
        draw_ui();

        EndDrawing();

    }

    LINFO("closed raylib window");
    shutdown_graphics();
    shutdown_ui();
    shutdown_network();
    shutdown_events();

    //TODO: disconnect socket???
    return 0;
}
