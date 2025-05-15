#include "ui.h"

#include "../../include/raylib.h"

#include "game_data.h"
#include "item_data.h"
#include "../tools/asserts.h"
#include "../tools/event.h"
#include "../tools/logger.h"

#include <stdio.h>
#include <string.h>

enum {
    UI_STATE_DISPLAY,
    UI_STATE_INPUT,
    UI_NUM_STATES
};

static ui_state* ui = 0;

u8 on_rdy_for_input(i32 event_type, void* listener, void* sender, event_data data); 
u8 on_anim_complete(i32 event_type, void* listener, void* sender, event_data data); 

void process_click(i32 button);
void process_key(i32 key);

u8 startup_ui(char* mechname) {

    SM_ASSERT(ui == 0);
    ui = (ui_state*)malloc(sizeof(ui_state));
    SM_ASSERT(ui != 0);

    ui->state = UI_STATE_DISPLAY;
    ui->data = init_game_data();

    if(ui->data == 0) {
        LERROR("failed to initialize game data.");
        return false;
    }
    if(!load_item_data()) {
        LERROR("failed to load item data.");
        return false;
    }
    if(!init_game(mechname)) {
        LERROR("failed to initialize game state.");
        return false;
    }
    
    register_event(EVENT_NET_READY_FOR_INPUT, on_rdy_for_input, ui);  
    register_event(EVENT_ANIM_COMPLETE, on_anim_complete, ui);  

    LINFO("user interface startup complete.");
    LUI("UI_STATE_DISPLAY");

    return true;
}

void shutdown_ui() {
    SM_ASSERT(ui != 0);
    free_game_data();
    free(ui);
    ui = 0;
    LINFO("shutdown ui.");
}

u8 update_ui() {
    switch(ui->state) {
        case UI_STATE_DISPLAY: break; //do nothing     
        case UI_STATE_INPUT:
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { process_click(0); } 
            else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) { process_click(1); } 
            else if(IsKeyPressed(KEY_A)) { process_key(KEY_A); }
            else if(IsKeyPressed(KEY_D)) { process_key(KEY_D); }
            else if(IsKeyPressed(KEY_Q)) { process_key(KEY_Q); }
            else if(IsKeyPressed(KEY_E)) { process_key(KEY_E); }
            else if(IsKeyPressed(KEY_ONE)) { process_key(KEY_ONE); }
            else if(IsKeyPressed(KEY_TWO)) { process_key(KEY_TWO); }
            else if(IsKeyPressed(KEY_THREE)) { process_key(KEY_THREE); }
            else if(IsKeyPressed(KEY_FOUR)) { process_key(KEY_FOUR); }
            else if(IsKeyPressed(KEY_FIVE)) { process_key(KEY_FIVE); }
            else if(IsKeyPressed(KEY_SIX)) { process_key(KEY_SIX); }
            else if(IsKeyPressed(KEY_S)) { process_key(KEY_S); }
            else if(IsKeyPressed(KEY_G)) { process_key(KEY_G); }
            else {}
            break;

    }
    return true;
}

void draw_ui() {
    char player_health[50];
    char enemy_health[50];
    if(ui->state == UI_STATE_INPUT) {
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/10, BLUE); 
    }
    else {
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/10, GRAY); 
    }
    snprintf(player_health, 50, "hp: %d", ui->data->d_dynamic[P1].hp);
    snprintf(enemy_health, 50, "hp: %d", ui->data->d_dynamic[P2].hp);
    DrawText("YOU:", 10, 10, 30, GREEN);
    DrawText(player_health, 200, 20, 20, GREEN);
    DrawText("BAD GUY:", WINDOW_WIDTH/2 + 10, 10, 30, RED);
    DrawText(enemy_health, WINDOW_WIDTH/2 + 200, 20, 20, RED);
}

u8 on_rdy_for_input(i32 event_type, void* listener, void* sender, event_data data) {
    ui->state = UI_STATE_INPUT; 
    LUI("UI_STATE_INPUT");
}

u8 on_anim_complete(i32 event_type, void* listener, void* sender, event_data data) {
    LANIM("animation complete.");
    //TODO: update game_data values
    switch(data.data[1]) {
        case ACTION_R1:
            process_command("r1");
            break;
        case ACTION_L1:
            process_command("l1");
            break;
        case ACTION_R2:
            process_command("r2");
            break;
        case ACTION_L2:
            process_command("l2");
            break;
        case ACTION_R3:
            process_command("r3");
            break;
        case ACTION_L3:
            process_command("l3");
            break;
        case ACTION_W1:    
            process_command("w1");
            break;
        case ACTION_W2:    
            process_command("w2");
            break;
        case ACTION_W3:    
            process_command("w3");
            break;
        case ACTION_W4:    
            process_command("w4");
            break;
        case ACTION_W5:    
            process_command("w5");
            break;
        case ACTION_W6:    
            process_command("w6");
            break;
        case ACTION_S:    
            process_command("s");
            break;
        case ACTION_G:    
            process_command("g");
            break;
    }
    event_data e;
    e.data[0] = 0;
    e.data[1] = 0;
    //TODO: temp, this will be somewhere else?
    LUI("animation complete.");
    fire_event(EVENT_UI_COMPLETE, NULL, e); 
}

void process_click(i32 button) {
    event_data d;
    if(button == 0) {
        d.data[0] = ACTION_L1;
        d.data[1] = 0;
    }
    if(button == 1) {
        d.data[0] = ACTION_R1;
        d.data[1] = 0;
    }
    fire_event(EVENT_UI_GOT_INPUT, NULL, d);    
    ui->state = UI_STATE_DISPLAY
    LUI("UI_STATE_DISPLAY");
}

void process_key(i32 key) {
    event_data d;
    d.data[1] = 0;
    char namebuf[50];
    switch(key) { 
        case KEY_A:
            d.data[0] = ACTION_L2;
            break;
        case KEY_D:
            d.data[0] = ACTION_R2;
            break;
        case KEY_Q:
            d.data[0] = ACTION_L3;
            break;
        case KEY_E:
            d.data[0] = ACTION_R3;
            break;
        case KEY_ONE:
            get_weapon_name(P1, namebuf, 0);
            if(strcmp(namebuf, "NONE") == 0) {
                LWARN("tried to use weapon 0 when it doesn't exist");
                return;
            }
            d.data[0] = ACTION_W1;
            break;
        case KEY_TWO:
            get_weapon_name(P1, namebuf, 1);
            if(strcmp(namebuf, "NONE") == 0) {
                LWARN("tried to use weapon 1 when it doesn't exist");
                return;
            }
            d.data[0] = ACTION_W2;
            break;
        case KEY_THREE:
            get_weapon_name(P1, namebuf, 2);
            if(strcmp(namebuf, "NONE") == 0) {
                LWARN("tried to use weapon 2 when it doesn't exist");
                return;
            }
            d.data[0] = ACTION_W3;
            break;
        case KEY_FOUR:
            get_weapon_name(P1, namebuf, 3);
            if(strcmp(namebuf, "NONE") == 0) {
                LWARN("tried to use weapon 3 when it doesn't exist");
                return;
            }
            d.data[0] = ACTION_W4;
            break;
        case KEY_FIVE:
            get_weapon_name(P1, namebuf, 4);
            if(strcmp(namebuf, "NONE") == 0) {
                LWARN("tried to use weapon 4 when it doesn't exist");
                return;
            }
            d.data[0] = ACTION_W5;
            break;
        case KEY_SIX:
            get_weapon_name(P1, namebuf, 5);
            if(strcmp(namebuf, "NONE") == 0) {
                LWARN("tried to use weapon 5 when it doesn't exist");
                return;
            }
            d.data[0] = ACTION_W6;
            break;
        case KEY_S:
            d.data[0] = ACTION_S;
            break;
        case KEY_G:
            d.data[0] = ACTION_G;
            break;
    }
    fire_event(EVENT_UI_GOT_INPUT, NULL, d);
    ui->state = UI_STATE_DISPLAY
    LUI("UI_STATE_DISPLAY");
}
