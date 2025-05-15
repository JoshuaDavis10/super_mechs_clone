#include "graphics.h"

#include "animation.h"
#include "game_data.h"
#include "../tools/asserts.h"
#include "../tools/event.h"
#include "../tools/utils.h"

#include <stdlib.h>
#include <stdio.h>

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

    graphics->abs_pos[P1] = (float)get_pos(P1);
    graphics->abs_pos[P2] = (float)get_pos(P2);
    graphics->target_pos[P1] = (float)get_pos(P1);
    graphics->target_pos[P2] = (float)get_pos(P2);
    graphics->drone_active[P1] = false;
    graphics->drone_active[P2] = false;
    graphics->state = GRAPHICS_STATE_STATIC;
    graphics->action = -1; //in static mode
    graphics->p_turn = -1; //in static mode
    graphics->p_not_turn = -1;


    char torso_path[1000];
    char leg_path[1000];
    char weapon_one_path[1000];

    get_drone_name(P1, graphics->drone_name[P1]);
    get_leg_name(P1, graphics->leg_name[P1]);
    get_torso_name(P1, graphics->torso_name[P1]);
    get_weapon_name(P1, graphics->weapon_names[P1][0], 0);

    get_drone_name(P2, graphics->drone_name[P2]);
    get_leg_name(P2, graphics->leg_name[P2]);
    get_torso_name(P2, graphics->torso_name[P2]);
    get_weapon_name(P2, graphics->weapon_names[P2][0], 0);

    LDEBUG("GRAPHICS STARTUP -> P1 drone name = %s", graphics->drone_name[P1]);
    LDEBUG("GRAPHICS STARTUP -> P1 leg name = %s", graphics->leg_name[P1]);
    LDEBUG("GRAPHICS STARTUP -> P1 torso name = %s", graphics->torso_name[P1]);
    LDEBUG("GRAPHICS STARTUP -> P1 weapon 1 name = %s", graphics->weapon_names[P1][0]);

    LDEBUG("GRAPHICS STARTUP -> P2 drone name = %s", graphics->drone_name[P2]);
    LDEBUG("GRAPHICS STARTUP -> P2 leg name = %s", graphics->leg_name[P2]);
    LDEBUG("GRAPHICS STARTUP -> P2 torso name = %s", graphics->torso_name[P2]);
    LDEBUG("GRAPHICS STARTUP -> P2 weapon 1 name = %s", graphics->weapon_names[P2][0]);

    snprintf(torso_path, 1000, "data/images/static/%s/%s.png", graphics->torso_name[P1], graphics->torso_name[P1]);
    snprintf(leg_path, 1000, "data/images/static/%s/%s.png", graphics->leg_name[P1], graphics->leg_name[P1]);
    snprintf(weapon_one_path, 1000, "data/images/static/%s/%s.png", graphics->weapon_names[P1][0], graphics->weapon_names[P1][0]);

    graphics->legs[P1] = LoadTexture(leg_path);
    graphics->torso[P1] = LoadTexture(torso_path);
    graphics->weapons[P1][0] = LoadTexture(weapon_one_path);

    snprintf(torso_path, 1000, "data/images/static/%s/%s.png", graphics->torso_name[P2], graphics->torso_name[P2]);
    snprintf(leg_path, 1000, "data/images/static/%s/%s.png", graphics->leg_name[P2], graphics->leg_name[P2]);
    snprintf(weapon_one_path, 1000, "data/images/static/%s/%s.png", graphics->weapon_names[P2][0], graphics->weapon_names[P2][0]);

    graphics->legs[P2] = LoadTexture(leg_path);
    graphics->torso[P2] = LoadTexture(torso_path);
    graphics->weapons[P2][0] = LoadTexture(weapon_one_path);

    startup_animation_system(graphics);

    register_event(EVENT_ACTION_TAKEN, on_action_event, graphics);

    return true;
}

void shutdown_graphics() {
    SM_ASSERT(graphics != 0);
    shutdown_animation_system();
    free(graphics);
    graphics = 0;
    LINFO("shutdown graphics.");
}

void draw_graphics() {

    if(graphics->state == GRAPHICS_STATE_STATIC) {
        //TODO: draw torso, legs, and weapons, and drone if activated
        DrawTexture(graphics->legs[P1], graphics->abs_pos[P1] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, BLUE); 
        DrawTexture(graphics->torso[P1], graphics->abs_pos[P1] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 192, BLUE); 
        DrawTexture(graphics->weapons[P1][0], graphics->abs_pos[P1] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, BLUE); 

        DrawTexture(graphics->legs[P2], graphics->abs_pos[P2] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, ORANGE); 
        DrawTexture(graphics->torso[P2], graphics->abs_pos[P2] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 192, ORANGE); 
        DrawTexture(graphics->weapons[P2][0], graphics->abs_pos[P2] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, ORANGE); 
    }
    else {
        double time_elapsed = timer_get_time(graphics->animation_timer);
        if(time_elapsed >= graphics->animation_timer.length) {
           event_data e;
           e.data[0] = graphics->p_turn;
           e.data[1] = graphics->action;
           timer_stop(&(graphics->animation_timer));
           graphics->state = GRAPHICS_STATE_STATIC;
           graphics->action = -1;
           graphics->abs_pos[graphics->p_not_turn] = graphics->target_pos[graphics->p_not_turn];
           graphics->abs_pos[graphics->p_turn] = graphics->target_pos[graphics->p_turn];
           fire_event(EVENT_ANIM_COMPLETE, NULL, e);
        }
        switch(graphics->action) {
            case ACTION_L1:
            case ACTION_L3:
            case ACTION_R1:
            case ACTION_R3:
                animate_walk();
                break;
            case ACTION_L2:
            case ACTION_R2:
                animate_jump();
                break;
            case ACTION_W1:
            case ACTION_W2:
            case ACTION_W3:
            case ACTION_W4:
            case ACTION_W5:
            case ACTION_W6:
                animate_weapon(graphics->action - ACTION_W1); //NOTE: passing in weapon being used
                break;
            case ACTION_S:
                animate_stomp();
                break;
            case ACTION_G:
                animate_grapple();
                break;
        }
    }

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
        case ACTION_W5:
        case ACTION_W6:
            if(diff < 0) { diff = -diff; }
            timer_start(&(g->animation_timer), diff/5);
            LANIM("timer length: %lf", diff/5); 
            break;
        case ACTION_S:
            //NOTE: this assumes stomp is always knockback
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
    LANIM("animating");
    return true;
}
