#include "graphics.h"

#include "../../include/raylib.h"

#include "game_data.h"
#include "../tools/asserts.h"
#include "../tools/event.h"
#include "../tools/utils.h"

#include <stdlib.h>

enum {
    GRAPHICS_STATE_STATIC,
    GRAPHICS_STATE_ANIMATING
};

static graphics_state* graphics = 0;

u8 on_action_event(i32 event_type, void* listener, void* sender, event_data data);

void draw_mech(float x, float y, i32 player);
void draw_bullet(float pos);

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
    graphics->state = GRAPHICS_STATE_STATIC;
    graphics->bullet_pos = -1.0f; //no bullet
    graphics->bullet_dir = -1; //no bullet
    graphics->action = -1; //in static mode
    graphics->p_turn = -1; //in static mode
    graphics->p_not_turn = -1;

    register_event(EVENT_ACTION_TAKEN, on_action_event, graphics);

    return true;
}

void shutdown_graphics() {
    SM_ASSERT(graphics != 0);
    free(graphics);
    graphics = 0;
    LINFO("shutdown graphics.");
}

u8 update_graphics() {
    //TODO: split updates based on action into diff functions?
    double time_elapsed = timer_get_time(graphics->animation_timer);
    float time;
    if(graphics->state == GRAPHICS_STATE_STATIC) { return true; }
    switch(graphics->action) {
        case -1:
            SM_ASSERT_MSG(0, "GRAPHICS_STATE_STATIC, should not have reached this line!");
            //do nothing
            break;
        case ACTION_R1:
        case ACTION_R2:
        case ACTION_R3:
        case ACTION_L1:
        case ACTION_L2:
        case ACTION_L3:
            if(graphics->abs_pos[graphics->p_turn] >= graphics->target_pos[graphics->p_turn]) { graphics->abs_pos[graphics->p_turn] = graphics->target_pos[graphics->p_turn] + (graphics->animation_timer.length - time_elapsed); }
            if(graphics->abs_pos[graphics->p_turn] < graphics->target_pos[graphics->p_turn]) { graphics->abs_pos[graphics->p_turn] = graphics->target_pos[graphics->p_turn] - (graphics->animation_timer.length - time_elapsed); }
            break;
        case ACTION_W1:
        case ACTION_W2:
        case ACTION_W3:
        case ACTION_W4:
            if(graphics->bullet_dir == 0) { graphics->bullet_pos = graphics->abs_pos[graphics->p_turn] - (time_elapsed*5); }
            if(graphics->bullet_dir == 1) { graphics->bullet_pos = graphics->abs_pos[graphics->p_turn] + (time_elapsed*5); }
            break;
        case ACTION_S:
            if(time_elapsed >= (2*graphics->animation_timer.length/3)) {
                time = graphics->animation_timer.length - time_elapsed; 
                if(graphics->abs_pos[graphics->p_not_turn] < graphics->target_pos[graphics->p_not_turn]) { graphics->abs_pos[graphics->p_not_turn] = graphics->target_pos[graphics->p_not_turn] - (time*2); }
                if(graphics->abs_pos[graphics->p_not_turn] > graphics->target_pos[graphics->p_not_turn]) { graphics->abs_pos[graphics->p_not_turn] = graphics->target_pos[graphics->p_not_turn] + (time*2); }
            }
            break;
        case ACTION_G:
            float diff = graphics->abs_pos[graphics->p_turn] - graphics->abs_pos[graphics->p_not_turn];
            if(time_elapsed >= graphics->animation_timer.length/2) {
                time = graphics->animation_timer.length - time_elapsed;
                if(diff < 0) { graphics->abs_pos[graphics->p_turn] = graphics->target_pos[graphics->p_turn] - (time * 6); }
                if(diff > 0) { graphics->abs_pos[graphics->p_turn] = graphics->target_pos[graphics->p_turn] + (time * 6); }
            }
            break;
    }

    if(time_elapsed >= graphics->animation_timer.length) {
       event_data e;
       e.data[0] = graphics->p_turn;
       e.data[1] = graphics->action;
       timer_stop(&(graphics->animation_timer));
       graphics->state = GRAPHICS_STATE_STATIC;
       graphics->bullet_pos = -1.0f;
       graphics->bullet_dir = -1;
       graphics->action = -1;
       graphics->abs_pos[graphics->p_not_turn] = graphics->target_pos[graphics->p_not_turn];
       graphics->abs_pos[graphics->p_turn] = graphics->target_pos[graphics->p_turn];
       fire_event(EVENT_ANIM_COMPLETE, NULL, e);
    }
    return true;
}

void draw_graphics() {
    //TODO: split draws based on action into diff functions
    float height;
    float time = timer_get_time(graphics->animation_timer);
    switch(graphics->action) {
        case ACTION_R2:    
        case ACTION_L2:    
            height = -2*(time - 1)*(time - 1) + 2;
            draw_mech((graphics->abs_pos[graphics->p_turn]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5 - height * 100, graphics->p_turn);
            draw_mech((graphics->abs_pos[graphics->p_not_turn]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, graphics->p_not_turn);
            break;
        case ACTION_W1:    
        case ACTION_W2:    
        case ACTION_W3:    
        case ACTION_W4:    
            draw_mech((graphics->abs_pos[P1]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, P1);
            draw_mech((graphics->abs_pos[P2]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, P2);
            draw_bullet(graphics->bullet_pos);
            break;
        case ACTION_R1:
        case ACTION_R3:
        case ACTION_L1:
        case ACTION_L3:
        case -1:
            //static state
            draw_mech((graphics->abs_pos[P1]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, P1);
            draw_mech((graphics->abs_pos[P2]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, P2);
            break;
        case ACTION_S:
            height = -4*(time - 0.5)*(time - 0.5) + 1;
            if(time >= 2*graphics->animation_timer.length/3) { height = 0.0f; }
            draw_mech((graphics->abs_pos[graphics->p_turn]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5 - (height) * 20, graphics->p_turn);
            draw_mech((graphics->abs_pos[graphics->p_not_turn]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, graphics->p_not_turn);
            break;
        case ACTION_G:
            if(time < graphics->animation_timer.length/2) {
                if(graphics->abs_pos[graphics->p_turn] < graphics->target_pos[graphics->p_turn]) {
                    DrawRectangle(graphics->abs_pos[graphics->p_turn]*100 + 50, 7*WINDOW_HEIGHT/8, time * 7 * 100 + 50, 5, GRAY); 
                }
                if(graphics->abs_pos[graphics->p_turn] > graphics->target_pos[graphics->p_turn]) {
                    DrawRectangle((graphics->abs_pos[graphics->p_turn]*100+50)-(time*7*100), 7*WINDOW_HEIGHT/8, time * 7 * 100, 5, GRAY); 
                }
            }
            else if(time > graphics->animation_timer.length/2) {
                if(graphics->abs_pos[graphics->p_turn] < graphics->target_pos[graphics->p_turn]) {
                    DrawRectangle(graphics->abs_pos[graphics->p_turn]*100 + 50, 7*WINDOW_HEIGHT/8, (graphics->target_pos[graphics->p_turn] - graphics->abs_pos[graphics->p_turn])*100 + 50, 5, GRAY); 
                }
                if(graphics->abs_pos[graphics->p_turn] > graphics->target_pos[graphics->p_turn]) {
                    DrawRectangle(graphics->target_pos[graphics->p_turn]*100, 7*WINDOW_HEIGHT/8, (graphics->abs_pos[graphics->p_turn] - graphics->target_pos[graphics->p_turn])*100, 5, GRAY);
                }
            }
            draw_mech((graphics->abs_pos[P1]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, P1);
            draw_mech((graphics->abs_pos[P2]*WINDOW_WIDTH/10), 4*WINDOW_HEIGHT/5, P2);
            break;
        default:    
            SM_ASSERT_MSG(0, "somehow reached default branch of draw_graphics switch");
            break;
    }
}

void draw_mech(float x, float y, i32 player) {
    if(player == P1) {
        DrawRectangle(x+35, y-30, 30, 30, GREEN); 
        DrawRectangle(x+40, y-20, 5, 5, WHITE); 
        DrawRectangle(x+55, y-20, 5, 5, WHITE); 
        DrawRectangle(x+20, y, 60, 60, GREEN); 
        DrawRectangle(x+20, y+60, 20, 40, GREEN); 
        DrawRectangle(x+60, y+60, 20, 40, GREEN); 
        DrawRectangle(x, y+10, 20, 40, GREEN); 
        DrawRectangle(x+80, y+10, 20, 40, GREEN); 
    }
    if(player == P2) {
        DrawRectangle(x+35, y-30, 30, 30, RED); 
        DrawRectangle(x+40, y-20, 5, 5, WHITE); 
        DrawRectangle(x+55, y-20, 5, 5, WHITE); 
        DrawRectangle(x+20, y, 60, 60, RED); 
        DrawRectangle(x+20, y+60, 20, 40, RED); 
        DrawRectangle(x+60, y+60, 20, 40, RED); 
        DrawRectangle(x, y+10, 20, 40, RED); 
        DrawRectangle(x+80, y+10, 20, 40, RED); 
    }
}

void draw_bullet(float pos) {
    DrawRectangle(pos*WINDOW_WIDTH/10+50, 7*WINDOW_HEIGHT/8, 20, 4, BLUE);
}

u8 on_action_event(i32 event_type, void* listener, void* sender, event_data data) {
    graphics_state* g = (graphics_state*)listener;
    i32 p_id = data.data[0];
    i32 action = data.data[1];
    i32 opp_id;
    if(p_id == P1) {opp_id = P2;}
    if(p_id == P2) {opp_id = P1;}
    float diff = g->abs_pos[p_id] - g->abs_pos[opp_id];
    switch(action) {
        case ACTION_R1:
            g->target_pos[p_id] = g->abs_pos[p_id] + 1.0f;
            timer_start(&(g->animation_timer), 1.0f);
            break;
        case ACTION_R2:
            g->target_pos[p_id] = g->abs_pos[p_id] + 2.0f;
            timer_start(&(g->animation_timer), 2.0f);
            break;
        case ACTION_R3:
            g->target_pos[p_id] = g->abs_pos[p_id] + 3.0f;
            timer_start(&(g->animation_timer), 3.0f);
            break;
        case ACTION_L1:
            g->target_pos[p_id] = g->abs_pos[p_id] - 1.0f;
            timer_start(&(g->animation_timer), 1.0f);
            break;
        case ACTION_L2:
            g->target_pos[p_id] = g->abs_pos[p_id] - 2.0f;
            timer_start(&(g->animation_timer), 2.0f);
            break;
        case ACTION_L3:
            g->target_pos[p_id] = g->abs_pos[p_id] - 3.0f;
            timer_start(&(g->animation_timer), 3.0f);
            break;
        case ACTION_W1:
        case ACTION_W2:
        case ACTION_W3:
        case ACTION_W4:
            if(diff < 0) { graphics->bullet_dir = 1; diff = -diff; g->bullet_pos = g->abs_pos[p_id]; } //dir right
            else if(diff > 0) { graphics->bullet_dir = 0; g->bullet_pos = g->abs_pos[p_id]; } //dir left 
            timer_start(&(g->animation_timer), diff/5);
            break;
        case ACTION_S:
            //TODO: we are assuming knockback occurs
            if(diff > 1.0f || diff < -1.0f) { SM_ASSERT_MSG(0, "tried to stomp when players were not next to each other"); }
            if(diff < 0) { g->target_pos[opp_id] += 1.0f; }
            if(diff > 0) { g->target_pos[opp_id] -= 1.0f; }
            timer_start(&(g->animation_timer), 1.5f);
            break;
        case ACTION_G:
            if(diff < 0) { diff = -diff; g->target_pos[p_id] = g->abs_pos[opp_id] - 1.0f;}
            else if(diff > 0) { g->target_pos[p_id] = g->abs_pos[opp_id] + 1.0f;}
            
            timer_start(&(g->animation_timer), (diff-1.0f)/3);
            break;
    }
    graphics->action = action;
    graphics->p_turn = p_id;
    graphics->p_not_turn = opp_id;
    graphics->state = GRAPHICS_STATE_ANIMATING;
    return true;
}
