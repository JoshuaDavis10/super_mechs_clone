#pragma once

#include "defines.h"

typedef struct item_torso {
    u32 weight;
    //TODO: data
} item_torso;

typedef struct item_leg {
    u32 weight;
    //TODO: data
} item_leg;

typedef struct item_side_weapon {
    u32 weight;
    //TODO: data
} item_side_weapon;

typedef struct item_top_weapon {
    u32 weight;
    //TODO: data
} item_top_weapon;

typedef struct item_teleport_module {
    u32 weight;
    //TODO: data
} item_teleport_module;

typedef struct item_charge_module {
    u32 weight;
    //TODO: data
} item_charge_module;

typedef struct item_grapple_module {
    u32 weight;
    //TODO: data
} item_grapple_module;

typedef struct item_drone {
    u32 weight;
    //TODO: data
} item_drone;

typedef struct item_module {
    u32 weight;
    //TODO: data
} item_module;

enum {
    I_TORSO,
    I_LEG,
    I_SIDE_WEAPON,
    I_TOP_WEAPON,
    I_TELEPORT_MODULE,
    I_CHARGE_MODULE,
    I_GRAPPLE_MODULE,
    I_DRONE,
    I_MODULE,
    I_TYPES_NUM
};

u8 load_item_data();
void free_item_data();
