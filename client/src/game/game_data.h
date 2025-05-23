#pragma once

#include "../defines.h"

typedef enum w_dmg_type {
    PHYS=0,
    HEAT=1,
    ENERGY=2
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
    char name[50];
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
   char drone_name[50];
   char leg_name[50];
   char torso_name[50];
   i32 max_hp;
   //TODO: names of all items that will be drawn: torso, legs, weapons, drone
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
    char* items[2];
} game_data;

//return 1 on successful initialize... for now it'll crash otherwise (assertion)
u8 init_game(const char* mech_name);
game_data* init_game_data(); 
u8 load_game_data(char* items_list_player, char* items_list_opp, i32 pos1, i32 pos2, i32 turn);
void free_game_data(); 
i8 process_command(const char* cmd); 
u32 get_turn();
i32 get_pos(i32 player);
void get_drone_name(i32 player, char* out_string);
void get_leg_name(i32 player, char* out_string);
void get_torso_name(i32 player, char* out_string);
void get_weapon_name(i32 player, char* out_string, i32 w_index);
