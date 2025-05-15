#pragma once

#include "../defines.h"
#include "graphics.h"

u8 startup_animation_system(graphics_state* graphics);
void shutdown_animation_system();
void animate_walk();
void animate_jump();
void animate_weapon(i32 index);
void animate_stomp();
void animate_grapple();
