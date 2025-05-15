#include "animation.h"

#include "../../include/raylib.h"

#include "../tools/asserts.h"
#include "../tools/darray.h"

#include <stdlib.h>
#include <string.h>

#define ANIMATION_SECONDS_PER_FRAME 0.15

typedef struct animation_data {
    graphics_state* graphics;
    Texture2D* walk_frames;
    Texture2D* jump_frames;
    Texture2D* weapon_frames[6];
    Texture2D* stomp_frames;
    Texture2D* grapple_frames;
    //TODO: textures for all animations
} animation_data;

static animation_data* a_data = 0;

u8 startup_animation_system(graphics_state* graphics) {

    SM_ASSERT(a_data == 0);
    a_data = (animation_data*)malloc(sizeof(animation_data));
    memset(a_data, 0, sizeof(animation_data));

    a_data->graphics = graphics;
    a_data->walk_frames = (Texture2D*)darray_create(sizeof(Texture2D));
    a_data->jump_frames = (Texture2D*)darray_create(sizeof(Texture2D));
    for(i32 i = 0; i < 6; i++) {
        a_data->weapon_frames[i] = (Texture2D*)darray_create(sizeof(Texture2D));
    }
    a_data->stomp_frames = (Texture2D*)darray_create(sizeof(Texture2D));
    a_data->grapple_frames = (Texture2D*)darray_create(sizeof(Texture2D));

    //TODO: load animation textures from graphics_state. names for all the items with animations
    Texture2D temp_walk_frames[4];
    temp_walk_frames[0] = LoadTexture("data/images/animation/thunder_thighs/walk/legs 2.png"); 
    temp_walk_frames[1] = LoadTexture("data/images/animation/thunder_thighs/walk/legs.png");
    temp_walk_frames[2] = LoadTexture("data/images/animation/thunder_thighs/walk/legs 3.png"); 
    temp_walk_frames[3] = LoadTexture("data/images/animation/thunder_thighs/walk/legs.png");
    darray_add(a_data->walk_frames, temp_walk_frames[0]);
    darray_add(a_data->walk_frames, temp_walk_frames[1]);
    darray_add(a_data->walk_frames, temp_walk_frames[2]);
    darray_add(a_data->walk_frames, temp_walk_frames[3]);

    return true;
}

void shutdown_animation_system() {
    SM_ASSERT(a_data != 0);
    darray_destroy(a_data->walk_frames);
    darray_destroy(a_data->jump_frames);
    darray_destroy(a_data->weapon_frames);
    darray_destroy(a_data->stomp_frames);
    darray_destroy(a_data->grapple_frames);
    free(a_data);
}

void animate_walk() {
    //temp vars
    float pos[2];
    float tar[2];
    i32 turn = a_data->graphics->p_turn;
    i32 not_turn = a_data->graphics->p_not_turn;
    i32 direction;
    pos[P1] = a_data->graphics->abs_pos[P1];
    pos[P2] = a_data->graphics->abs_pos[P2];
    tar[P1] = a_data->graphics->target_pos[P1];
    tar[P2] = a_data->graphics->target_pos[P2];

    if(pos[turn] < tar[turn]) { direction = 1; }
    if(pos[turn] > tar[turn]) { direction =-1; }

    //update
    double time_elapsed = timer_get_time(a_data->graphics->animation_timer);
    i32 frame = ((i32)(time_elapsed / ANIMATION_SECONDS_PER_FRAME)) % darray_get_info(a_data->walk_frames, D_LENGTH); //time since start / seconds per frame = frames since start -> % #frames = current frame
    LANIM("drawing walk frame %d", frame);

    //draw
    switch(turn) {
        case P1:
            DrawTexture(a_data->walk_frames[frame], (pos[P1]+(direction*time_elapsed)) * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, BLUE);
            DrawTexture(a_data->graphics->torso[P1], (pos[P1]+(direction*time_elapsed)) * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 192, BLUE); 
            DrawTexture(a_data->graphics->weapons[P1][0], (pos[P1]+(direction*time_elapsed)) * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, BLUE); 

            DrawTexture(a_data->graphics->legs[P2], pos[P2] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, ORANGE); 
            DrawTexture(a_data->graphics->torso[P2], pos[P2] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 192, ORANGE); 
            DrawTexture(a_data->graphics->weapons[P2][0], pos[P2] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, ORANGE); 
            break;
        case P2:
            DrawTexture(a_data->graphics->legs[P1], pos[P1] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, BLUE); 
            DrawTexture(a_data->graphics->torso[P1], pos[P1] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 192, BLUE); 
            DrawTexture(a_data->graphics->weapons[P1][0], pos[P1] * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, BLUE); 

            DrawTexture(a_data->walk_frames[frame], (pos[P2]+(direction*time_elapsed)) * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, ORANGE);
            DrawTexture(a_data->graphics->torso[P2], (pos[P2]+(direction*time_elapsed)) * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 192, ORANGE); 
            DrawTexture(a_data->graphics->weapons[P2][0], (pos[P2]+(direction*time_elapsed)) * (WINDOW_WIDTH/10), WINDOW_HEIGHT - 128, ORANGE); 
            break;
    }
}

void animate_jump() {
    //TODO: code here
}

void animate_weapon(i32 index) {
    //TODO: code here
}

void animate_stomp() {
    //TODO: code here
}

void animate_grapple() {
    //TODO: code here
}
