#pragma once

#include "../defines.h"

typedef struct timer {
    float start_time;
    float length;
} timer;

float timer_get_time(timer timer);
void timer_start(timer* timer, float length);
void timer_stop(timer* timer);
u8 is_timer_done(timer timer);
