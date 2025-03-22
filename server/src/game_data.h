#pragma once

#include "defines.h"

typedef enum w_dmg_type {
    PHYS,
    HEAT,
    ENERGY
} w_dmg_type;

typedef enum w_displace {
    DISPLACE_KNOCKBACK=1,
    DISPLACE_KNOCKFORWARD=2,
    DISPLACE_NONE=0,
    DISPLACE_GRAPPLE=7,
    DISPLACE_CHARGE=8,
    DISPLACE_TELEPORT=9
} w_displace;

typedef struct weapon_data {
    i32 min_range;
    i32 max_range;
    i32 damage;
    i32 heat_cost;
    i32 heat_damage; //increase enemy heat 
    i32 max_heat_damage; //decrease enemy max heat
    i32 cooling_damage; //decrease enemy cooling
    i32 energy_cost;
    i32 energy_damage; //decrease enemy energy
    i32 max_energy_damage; //decrease enemy max energy
    i32 regen_damage; //decrease enemy energy regen
    i32 phys_res_dmg; //decrease enemy physical resistance
    i32 heat_res_dmg; //decrease enemy heat resistance
    i32 energy_res_dmg; //decrease enemy energy resistance
    w_displace displace;
    w_dmg_type dmg_type; 
} weapon_data;

typedef struct static_data {
   weapon_data weapons[11]; //weapons @ index 6-9 are stomp, grapple, charge, teleport (see enum @ top). weapon @ index 10 is drone
   i32 max_hp;
} static_data;

typedef struct dynamic_data {
    i32 weapon_uses[10];
    i32 stomp_uses;
    i32 hp;
    i32 pos;
    i32 heat;
    i32 max_heat;
    i32 cooling;
    i32 energy;
    i32 max_energy;
    i32 energy_regen;
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

//return 1 on successful initialize... for now it'll crash otherwise (assertion)
i8 init_game_data(); 
void free_game_data(); 

//return -1 on error (as in the request is valid but the action implied by the request can't be carried out)
//return 1 if P1 wins as a result of request
//return 2 if P2 wins as a result of request
//return 0 otherwise.
i8 process_command(const char* cmd); 
u32 get_turn();
