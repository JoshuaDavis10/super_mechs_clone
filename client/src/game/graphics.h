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
} graphics_state;

u8 startup_graphics();
void shutdown_graphics();
u8 update_graphics();
void draw_graphics();
