#pragma once

#include "../../include/raylib.h"

#include "../defines.h"

#include "../tools/timer.h"

typedef struct graphics_state {
    char weapon_names[2][6][50];
    Texture2D weapons[2][6]; //TODO: how will we know which of these are empty (1-4 is side weapons, 5-6 is top weapons)
    Texture2D drone[2]; //NOTE: we may not keep drones
    Texture2D legs[2];
    Texture2D torso[2];
    timer animation_timer;
    char drone_name[2][50];
    char leg_name[2][50];
    char torso_name[2][50];
    float abs_pos[2];
    float target_pos[2];
    i32 state;
    i32 action; //will be -1 when in static mode
    i32 p_turn; //will be -1 in static mode
    i32 p_not_turn; //will be -1 in static mode
    u8 drone_active[2];
} graphics_state;

u8 startup_graphics();
void shutdown_graphics();
u8 update_graphics();
void draw_graphics();
