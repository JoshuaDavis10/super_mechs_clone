#include "timer.h"

#include "logger.h"

#include <windows.h>

float timer_get_time(timer timer) {
    if(timer.start_time == -1.0f) {
        return -1.0f;
    }
    ULONGLONG milliseconds = GetTickCount64();
    float seconds = (float)milliseconds / 1000.0f;
    float return_time = seconds - timer.start_time;
    return return_time;
}

void timer_start(timer* timer, float length) {
    ULONGLONG milliseconds = GetTickCount64();
    float seconds = (float)milliseconds / 1000.0f;
    timer->start_time = seconds;
    timer->length = length;
}

void timer_stop(timer* timer) {
    timer->start_time = -1.0f;
    timer->length = -1.0f;
}
