#pragma once

#include "defines.h"

enum {
    EVENT_LEFT_CLICK,
    EVENT_RIGHT_CLICK,
    EVENT_ACTION_TAKEN,
    EVENT_NET_READY_FOR_INPUT,
    EVENT_UI_GOT_INPUT,
    EVENT_UI_COMPLETE,
    EVENT_ANIM_COMPLETE,
    EVENT_NUM_TYPES
};

typedef struct event_data {
    i32 data[2];
} event_data;

typedef u8 (*callback)(i32 event_type, void* listener, void* sender, event_data data);

u8 startup_events(); 
void shutdown_events(); 
void register_event(i32 event_type, callback on_event, void* listener); 
void fire_event(i32 event_type, void* sender, event_data data); 
