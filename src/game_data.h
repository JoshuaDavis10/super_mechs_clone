#pragma once

#include "defines.h"

enum {
    TELEPORT,
    CHARGE,
    GRAPPLE
};

typedef enum w_dmg_type {
    PHYS,
    HEAT,
    ENERGY
} w_dmg_type;

typedef enum w_displace {
    KNOCKBACK,
    KNOCKFORWARD
} w_displace;

typedef struct weapon_data {
    u32 min_range;
    u32 max_range;
    u32 damage;
    u32 heat_cost;
    u32 heat_damage; //increase enemy heat 
    u32 max_heat_damage; //decrease enemy max heat
    u32 cooling_damage; //decrease enemy cooling
    u32 energy_cost;
    u32 energy_damage; //decrease enemy energy
    u32 max_energy_damage; //decrease enemy max energy
    u32 regen_damage; //decrease enemy energy regen
    u32 phys_res_dmg; //decrease enemy physical resistance
    u32 heat_res_dmg; //decrease enemy heat resistance
    u32 energy_res_dmg; //decrease enemy energy resistance
    w_displace displace;
    w_dmg_type dmg_type; 
} weapon_data;

typedef struct static_data {
   weapon_data weapons[6]; 
   u32 max_hp;
} static_data;

typedef struct dynamic_data {
    u32 weapon_uses[6];
    u32 special_uses[3];
    u32 stomp_uses;
    i32 hp;
    u32 pos;
    u32 heat;
    u32 max_heat;
    u32 cooling;
    u32 energy;
    u32 max_energy;
    u32 energy_regen;
    i32 phys_res;
    i32 heat_res;
    i32 energy_res;
    u8 drone_active;
} dynamic_data;

typedef struct game_data {
    static_data d_static[2]; //index 0 for P1's static data, index 1 for P2's static data
    dynamic_data d_dynamic[2]; //index 0 for P1's dynamic data, index 1 for P2's dynamic data
    u32 turn; //either P1 (0) or P2 (1)
    u32 actions; //actions remaining for current turn
} game_data;
