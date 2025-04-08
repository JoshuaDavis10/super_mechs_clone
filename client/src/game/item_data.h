#pragma once

#include "../defines.h"
#include "game_data.h"

typedef struct item_torso {
    i32 weight;
    i32 hp;
    i32 heat;
    i32 energy;
    i32 phys_res;
    i32 heat_res;
    i32 energy_res;
    i32 cooling;
    i32 energy_regen;
} item_torso;

typedef struct item_leg {
    i32 weight;
    i32 hp;
    i32 uses;
    i32 damage;
    i32 heat_cost;
    i32 heat_damage;
    i32 max_heat_damage;
    i32 cooling_damage;
    i32 energy_cost;
    i32 energy_damage;
    i32 max_energy_damage;
    i32 regen_damage;
    i32 phys_res_dmg;
    i32 heat_res_dmg;
    i32 energy_res_dmg;
    w_displace displace;
    w_dmg_type dmg_type;
} item_leg;

typedef struct item_side_weapon {
    i32 weight;
    i32 uses;
    i32 min_range;
    i32 max_range;
    i32 damage;
    i32 heat_cost;
    i32 heat_damage;
    i32 max_heat_damage;
    i32 cooling_damage;
    i32 energy_cost;
    i32 energy_damage;
    i32 max_energy_damage;
    i32 regen_damage;
    i32 phys_res_dmg;
    i32 heat_res_dmg;
    i32 energy_res_dmg;
    w_displace displace;
    w_dmg_type dmg_type;
} item_side_weapon;

typedef struct item_top_weapon {
    i32 weight;
    i32 uses;
    i32 min_range;
    i32 max_range;
    i32 damage;
    i32 heat_cost;
    i32 heat_damage;
    i32 max_heat_damage;
    i32 cooling_damage;
    i32 energy_cost;
    i32 energy_damage;
    i32 max_energy_damage;
    i32 regen_damage;
    i32 phys_res_dmg;
    i32 heat_res_dmg;
    i32 energy_res_dmg;
    w_displace displace;
    w_dmg_type dmg_type;
} item_top_weapon;

typedef struct item_teleport_module {
    i32 weight;
    i32 uses;
    i32 damage;
    i32 heat_cost;
    i32 heat_damage;
    i32 max_heat_damage;
    i32 cooling_damage;
    i32 energy_cost;
    i32 energy_damage;
    i32 max_energy_damage;
    i32 regen_damage;
    i32 phys_res_dmg;
    i32 heat_res_dmg;
    i32 energy_res_dmg;
    w_dmg_type dmg_type;
} item_teleport_module;

typedef struct item_charge_module {
    i32 weight;
    i32 uses;
    i32 damage;
    i32 heat_cost;
    i32 heat_damage;
    i32 max_heat_damage;
    i32 cooling_damage;
    i32 energy_cost;
    i32 energy_damage;
    i32 max_energy_damage;
    i32 regen_damage;
    i32 phys_res_dmg;
    i32 heat_res_dmg;
    i32 energy_res_dmg;
    w_dmg_type dmg_type;
} item_charge_module;

typedef struct item_grapple_module {
    i32 weight;
    i32 uses;
    i32 damage;
    i32 heat_cost;
    i32 heat_damage;
    i32 max_heat_damage;
    i32 cooling_damage;
    i32 energy_cost;
    i32 energy_damage;
    i32 max_energy_damage;
    i32 regen_damage;
    i32 phys_res_dmg;
    i32 heat_res_dmg;
    i32 energy_res_dmg;
    w_dmg_type dmg_type;
} item_grapple_module;

typedef struct item_drone {
    i32 weight;
    i32 damage;
    i32 heat_cost;
    i32 heat_damage;
    i32 max_heat_damage;
    i32 cooling_damage;
    i32 energy_cost;
    i32 energy_damage;
    i32 max_energy_damage;
    i32 regen_damage;
    i32 phys_res_dmg;
    i32 heat_res_dmg;
    i32 energy_res_dmg;
    w_dmg_type dmg_type;
} item_drone;

typedef struct item_module {
    i32 weight;
    i32 hp;
    i32 heat;
    i32 energy;
    i32 cooling;
    i32 energy_regen;
    i32 phys_res;
    i32 heat_res;
    i32 energy_res;
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

item_torso get_torso(i32 i_ID);
item_leg get_leg(i32 i_ID);
item_side_weapon get_side_weapon(i32 i_ID);
item_top_weapon get_top_weapon(i32 i_ID);
item_teleport_module get_teleport_module(i32 i_ID);
item_charge_module get_charge_module(i32 i_ID);
item_grapple_module get_grapple_module(i32 i_ID);
item_drone get_drone(i32 i_ID);
item_module get_module(i32 i_ID);
