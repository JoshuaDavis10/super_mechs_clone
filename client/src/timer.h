#pragma once

typedef struct timer {
    float start_time;
} timer;

float timer_get_time(timer timer);
void timer_start(timer* timer);
void timer_stop(timer* timer);
