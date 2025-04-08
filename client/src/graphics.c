#include "graphics.h"

#include "../include/raylib.h"

#include "game_data.h"
#include "asserts.h"
#include "event.h"

#include <stdlib.h>

enum {
    GRAPHICS_STATE_STATIC,
    GRAPHICS_STATE_MOVEMENT_P1,
    GRAPHICS_STATE_MOVEMENT_P2,
    GRAPHICS_NUM_STATES
};

static graphics_state* graphics = 0;

u8 on_action_event(i32 event_type, void* listener, void* sender, event_data data);

u8 startup_graphics() {
    SM_ASSERT(graphics == 0);
    graphics = (graphics_state*)malloc(sizeof(graphics_state));
    SM_ASSERT(graphics != 0);

    //TODO: load needed data to graphics state from game state
    graphics->abs_pos[P1] = (float)get_pos(P1);
    graphics->abs_pos[P2] = (float)get_pos(P2);
    graphics->target_pos[P1] = (float)get_pos(P1);
    graphics->target_pos[P2] = (float)get_pos(P2);
    graphics->drone_active[P1] = false;
    graphics->drone_active[P2] = false;
    graphics->drone_pos[P1] = -1.0f; //-1.0f for when drone isn't active
    graphics->drone_pos[P2] = -1.0f;

    register_event(EVENT_ACTION_TAKEN, on_action_event, graphics);

    return true;
}

void shutdown_graphics() {
    SM_ASSERT(graphics != 0);
    free(graphics);
    graphics = 0;
}

u8 update_graphics() {
    //TODO: do stuff based on graphics state
    double time_elapsed = timer_get_time(graphics->animation_timer);
    switch(graphics->state) {
        case GRAPHICS_STATE_STATIC:
            //do nothing
            break;
        case GRAPHICS_STATE_MOVEMENT_P1:
            if(graphics->abs_pos[P1] >= graphics->target_pos[P1]) { graphics->abs_pos[P1] = graphics->target_pos[P1] + (1.0 - time_elapsed); }
            if(graphics->abs_pos[P1] < graphics->target_pos[P1]) { graphics->abs_pos[P1] = graphics->target_pos[P1] - (1.0 - time_elapsed); }
            if(time_elapsed >= 1.0) {
                event_data e;
                e.data[0] = 0;
                e.data[1] = 0;
                timer_stop(&(graphics->animation_timer));
                graphics->state = GRAPHICS_STATE_STATIC;
                graphics->abs_pos[P1] = graphics->target_pos[P1];
                fire_event(EVENT_ANIM_COMPLETE, NULL, e);
            }
            break;
        case GRAPHICS_STATE_MOVEMENT_P2:
            if(graphics->abs_pos[P2] >= graphics->target_pos[P2]) { graphics->abs_pos[P2] = graphics->target_pos[P2] + (1.0 - time_elapsed); }
            if(graphics->abs_pos[P2] < graphics->target_pos[P2]) { graphics->abs_pos[P2] = graphics->target_pos[P2] - (1.0 - time_elapsed); }
            if(time_elapsed >= 1.0) {
                event_data e;
                e.data[0] = 0;
                e.data[1] = 0;
                timer_stop(&(graphics->animation_timer));
                graphics->state = GRAPHICS_STATE_STATIC;
                graphics->abs_pos[P2] = graphics->target_pos[P2];
                fire_event(EVENT_ANIM_COMPLETE, NULL, e); //TODO: this event's data needs info on the command that happened
            }
            break;
    }
    
    return true;
}

void draw_graphics() {
    //TODO: draw render state stuff
    DrawRectangle((graphics->abs_pos[P1]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, WINDOW_WIDTH/10, WINDOW_HEIGHT/5, GREEN);
    DrawRectangle((graphics->abs_pos[P2]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, WINDOW_WIDTH/10, WINDOW_HEIGHT/5, RED);
}

u8 on_action_event(i32 event_type, void* listener, void* sender, event_data data) {
    graphics_state* g = (graphics_state*)listener;
    i32 p_id = data.data[0];
    i32 action = data.data[1];
    switch(action) {
        case ACTION_R1:
            g->target_pos[p_id] = g->abs_pos[p_id] + 1.0f;
            break;
        case ACTION_L1:
            g->target_pos[p_id] = g->abs_pos[p_id] - 1.0f;
            break;
    }
    if(p_id == P1) { g->state = GRAPHICS_STATE_MOVEMENT_P1; timer_start(&(g->animation_timer)); }
    if(p_id == P2) { g->state = GRAPHICS_STATE_MOVEMENT_P2; timer_start(&(g->animation_timer)); }
    return true;
}
