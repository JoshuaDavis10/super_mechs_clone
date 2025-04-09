#pragma once

#include "../defines.h"

#include "../tools/timer.h"

typedef struct graphics_state {
    float abs_pos[2];
    float target_pos[2];
    u8 drone_active[2];
    float drone_pos[2];
    i32 state;
    timer animation_timer;
    float bullet_pos;
    i32 bullet_dir; //0 is left, 1 is right
    i32 action; //will be -1 when in static mode
    i32 p_turn; //will be -1 in static mode
    i32 p_not_turn; //will be -1 in static mode
} graphics_state;

u8 startup_graphics();
void shutdown_graphics();
u8 update_graphics();
void draw_graphics();
