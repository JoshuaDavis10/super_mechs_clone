#include "event.h"

#include "darray.h"
#include "asserts.h"

#include <stdlib.h>
#include <string.h>

typedef struct event {
    void* listener;
    callback on_event;
} event;

typedef struct events {
    event* events[EVENT_NUM_TYPES];
} events;

static events* events_state = 0;

u8 startup_events() {
    LDEBUG("no crash 1");
    SM_ASSERT(events_state == 0);
    events_state = (events*)malloc(sizeof(events));
    memset(events_state, 0, sizeof(events));
    SM_ASSERT(events_state != 0);
    LINFO("event system startup complete.");
    return true;
}

void shutdown_events() {
    for(int i = 0; i < EVENT_NUM_TYPES; i++) {
        if(events_state->events[i] != 0) {
            darray_destroy(events_state->events[i]);
        }
    }
    free(events_state);
}

void register_event(i32 event_type, callback on_event, void* listener) {
    if(events_state->events[event_type] == 0) {
        events_state->events[event_type] = darray_create(sizeof(event));
    }
    event event_to_register;
    event_to_register.listener = listener;
    event_to_register.on_event = on_event;
    darray_add(events_state->events[event_type], event_to_register);
}

void fire_event(i32 event_type, void* sender, event_data data) {
    if(events_state->events[event_type] == 0) {
        return; //TODO: temp... we should assert here once everything is stood up
    }
    u64 num_events = darray_get_info(events_state->events[event_type], D_LENGTH);
    event* events_list = events_state->events[event_type];
    for(int i = 0; i < num_events; i++) {
        events_list[i].on_event(event_type, events_list[i].listener, sender, data); 
    }
}
