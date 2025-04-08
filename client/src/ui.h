#pragma once

#include "defines.h"

#include "game_data.h"

typedef struct ui_state {
    i32 state;
    game_data* data;
    //TODO: buttons and stuff
} ui_state;

u8 startup_ui(char* mechname);
void shutdown_ui();
u8 update_ui();
void draw_ui();
