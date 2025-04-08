#include "ui.h"

#include "../../include/raylib.h"

#include "game_data.h"
#include "item_data.h"
#include "../tools/asserts.h"
#include "../tools/event.h"
#include "../tools/logger.h"

enum {
    UI_STATE_DISPLAY,
    UI_STATE_INPUT,
    UI_NUM_STATES
};

static ui_state* ui = 0;

u8 on_rdy_for_input(i32 event_type, void* listener, void* sender, event_data data); 
u8 on_anim_complete(i32 event_type, void* listener, void* sender, event_data data); 

void process_click(i32 button);

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
    free(ui);
    ui = 0;
}

u8 update_ui() {
    switch(ui->state) {
        case UI_STATE_DISPLAY: break; //do nothing     
        case UI_STATE_INPUT:
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { process_click(0); } 
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) { process_click(1); } 
            break;

    }
    return true;
}

void draw_ui() {
    //TODO: draw stuff based on state
    if(ui->state == UI_STATE_INPUT) {
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/10, BLUE); 
    }
    else {
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/10, GRAY); 
    }
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
