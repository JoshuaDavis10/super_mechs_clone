#include "game_data.h"

#include "asserts.h"
#include "logger.h"
#include "socket.h"
#include "item_data.h"
#include "utils.h"
#include "files.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_POSITIONS 10

enum {
    //weapon use requests
    REQ_W1 = 0, REQ_W2 = 1, REQ_W3 = 2, REQ_W4 = 3, REQ_W5 = 4, REQ_W6 = 5,
    //special use requests
    REQ_S = 6, REQ_G = 7, REQ_CH = 8, REQ_T0 = 9, REQ_T1 = 10, REQ_T2 = 11, REQ_T3 = 12, REQ_T4 = 13, 
    REQ_T5 = 14, REQ_T6 = 15, REQ_T7 = 16, REQ_T8 = 17, REQ_T9 = 18,
    REQ_DA = 19, REQ_DD = 20, REQ_C = 21,
    //movement requests
    REQ_R1 = 22, REQ_R2 = 23, REQ_R3 = 24, REQ_L1 = 25, REQ_L2 = 26, REQ_L3 = 27,
    //other
    REQ_F = 28, REQ_ER = 29, REQ_RC = 30, NUM_REQ_STRINGS = 31
};

const char* cmd_strings[] = 
{
"w1", "w2", "w3", "w4", "w5", "w6", "s", "g", "ch", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9", "da", "dd", "c",
"r1", "r2", "r3", "l1", "l2", "l3", "f", "er", "rc"
};

static game_data* g_data = 0;

i32 get_pos(i32 player) {
    return g_data->d_dynamic[player].pos;
}

u8 load_items_from_file(const char* filepath, char* items_list);
u8 send_items_to_server(const char* items_list); 
u8 get_items_from_server(char* items_list_opp); 
u8 load_game_state(char* items_list_player, char* items_list_opp); 

u8 init_game(const char* mech_name) {
    char items_buffer_player[MAX_MESSAGE_SIZE]; //TODO: 22 cringe constant
    char items_buffer_opp[MAX_MESSAGE_SIZE]; 

    if(!load_items_from_file(mech_name, items_buffer_player)) {
        LERROR("failed to read items from mech file");
        return false;
    }
    LTRACE("loaded items from mech file");

    if(!send_items_to_server(items_buffer_player)) {
        LERROR("failed to send items to server");
        return false;
    }
    LTRACE("sent player's items to server");


    if(!get_items_from_server(items_buffer_opp)) {
        LERROR("failed to get items from server");
        return false;
    }
    LTRACE("got opponent's items from server");

    if(!load_game_state(items_buffer_player, items_buffer_opp)) {
        LERROR("failed to load game state with item data");
        return false;
    }
    LTRACE("loaded game state with item data");

    LINFO("initialized game state");

    send_msg("start");

    return true;
}

u8 load_items_from_file(const char* mech_name, char* items_list) {
    file mechfile;
    char filepath[1000];
    snprintf(filepath, 1000, "mechfiles/%s.txt", mech_name);
    if(!open_file(filepath, &mechfile, "r")) {
        LERROR("failed to open file: %s", filepath);
        return false;
    }

    if(!read_line(mechfile, items_list, MAX_MESSAGE_SIZE * 22)) { //TODO: cringe constant   
        LERROR("failed to read line from file");
        return false;
    }

    LTRACE("read line: %s from mechfile", items_list);

    return true; 
}

u8 send_items_to_server(const char* items_list) {
    char msg_buffer[MAX_MESSAGE_SIZE];
    if(!send_msg(items_list)) {
        LERROR("failed to send message to server");
        return false;
    }
    if(!recv_msg(msg_buffer)) {
        LERROR("failed to receive message from server");
        return false;
    }
    if(strcmp(msg_buffer, "success") != 0) {
        LERROR("server message: %s", msg_buffer);
        return false;
    }

    return true;
}

u8 get_items_from_server(char* items_list_opp) {
    if(!recv_msg(items_list_opp)) {
        return false;
    }
    send_msg("ACK");
    return true;
}

u8 load_game_state(char* items_list_player, char* items_list_opp) {
    char pos_buffer[MAX_MESSAGE_SIZE];
    char turn_buffer[MAX_MESSAGE_SIZE];
    if(!recv_msg(pos_buffer)) { return false; }

    char *pos_str, *tok, *pos, *saveptr1, *saveptr2;
    i32 pos1, pos2;
    i32 turn;

    pos_str = strtok_r(pos_buffer, ",", &saveptr1);
    tok = strtok_r(pos_str, ":", &saveptr2);
    pos = strtok_r(NULL, ":", &saveptr2);
    SM_ASSERT(is_number(pos));
    pos1 = atoi(pos);

    pos_str = strtok_r(NULL, ",", &saveptr1);
    tok = strtok_r(pos_str, ":", &saveptr2);
    pos = strtok_r(NULL, ":", &saveptr2);
    SM_ASSERT(is_number(pos));
    pos2 = atoi(pos);

    send_msg("ACK");
    if(!recv_msg(turn_buffer)) { return false; }
    SM_ASSERT(is_number(turn_buffer));
    turn = atoi(turn_buffer);
    SM_ASSERT(turn == 0 || turn == 1);

    if(!load_game_data(items_list_player, items_list_opp, pos1, pos2, turn)) {
        return false;
    }
    return true;
}

game_data* init_game_data() {
    SM_ASSERT(g_data == 0);
    g_data = (game_data*)malloc(sizeof(game_data));
    memset(g_data, 0, sizeof(game_data)); 
    LINFO("game data initialized.");
    LTRACE("memory used for game data: %lu bytes", sizeof(*g_data));
    return g_data;
}

void free_game_data() {
    SM_ASSERT(g_data != 0);
    free(g_data);
    g_data = 0;
    LINFO("uninitialized game data.");
}

u32 get_turn() {
    return g_data->turn;
}

u8 load_torso_to_game_data(item_torso t, i32 player) {
    g_data->d_dynamic[player].hp += t.hp; 
    g_data->d_static[player].max_hp += t.hp;
    g_data->d_dynamic[player].max_heat += t.heat;
    g_data->d_dynamic[player].cooling += t.cooling;
    g_data->d_dynamic[player].energy += t.energy;
    g_data->d_dynamic[player].max_energy += t.energy;
    g_data->d_dynamic[player].energy_regen += t.energy_regen;
    g_data->d_dynamic[player].phys_res+= t.phys_res;
    g_data->d_dynamic[player].heat_res+= t.heat_res;
    g_data->d_dynamic[player].energy_res+= t.energy_res;
    return true;
}

u8 load_leg_to_game_data(item_leg t, i32 player) {
    g_data->d_dynamic[player].weapon_uses[REQ_S] = t.uses;
    g_data->d_dynamic[player].hp += t.hp;
    g_data->d_static[player].max_hp += t.hp;

    g_data->d_static[player].weapons[REQ_S].min_range = 1;
    g_data->d_static[player].weapons[REQ_S].max_range= 1;
    g_data->d_static[player].weapons[REQ_S].damage = t.damage;
    g_data->d_static[player].weapons[REQ_S].heat_cost = t.heat_cost;
    g_data->d_static[player].weapons[REQ_S].heat_damage = t.heat_damage;
    g_data->d_static[player].weapons[REQ_S].max_heat_damage = t.max_heat_damage;
    g_data->d_static[player].weapons[REQ_S].cooling_damage = t.cooling_damage;
    g_data->d_static[player].weapons[REQ_S].energy_cost = t.energy_cost;
    g_data->d_static[player].weapons[REQ_S].energy_damage = t.energy_damage;
    g_data->d_static[player].weapons[REQ_S].max_energy_damage = t.max_energy_damage;
    g_data->d_static[player].weapons[REQ_S].regen_damage = t.regen_damage;
    g_data->d_static[player].weapons[REQ_S].phys_res_dmg = t.phys_res_dmg;
    g_data->d_static[player].weapons[REQ_S].heat_res_dmg = t.heat_res_dmg;
    g_data->d_static[player].weapons[REQ_S].energy_res_dmg = t.energy_res_dmg;
    g_data->d_static[player].weapons[REQ_S].displace = t.displace;
    g_data->d_static[player].weapons[REQ_S].dmg_type = t.dmg_type;
    return true;
}

u8 load_side_weapon_to_game_data(item_side_weapon t, i32 player, i32 sw_count) {
    g_data->d_dynamic[player].weapon_uses[REQ_W1 + sw_count - 1] = t.uses;

    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].min_range = t.min_range;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].max_range = t.max_range;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].damage = t.damage;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].heat_cost = t.heat_cost;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].heat_damage = t.heat_damage;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].max_heat_damage = t.max_heat_damage;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].cooling_damage = t.cooling_damage;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].energy_cost = t.energy_cost;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].energy_damage = t.energy_damage;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].max_energy_damage = t.max_energy_damage;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].regen_damage = t.regen_damage;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].phys_res_dmg = t.phys_res_dmg;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].heat_res_dmg = t.heat_res_dmg;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].energy_res_dmg = t.energy_res_dmg;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].displace = t.displace;
    g_data->d_static[player].weapons[REQ_W1 + sw_count - 1].dmg_type = t.dmg_type;
    return true;
}

u8 load_top_weapon_to_game_data(item_top_weapon t, i32 player, i32 tw_count) {
    g_data->d_dynamic[player].weapon_uses[REQ_W5 + tw_count - 1] = t.uses;

    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].min_range = t.min_range;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].max_range = t.max_range;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].damage = t.damage;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].heat_cost = t.heat_cost;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].heat_damage = t.heat_damage;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].max_heat_damage = t.max_heat_damage;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].cooling_damage = t.cooling_damage;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].energy_cost = t.energy_cost;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].energy_damage = t.energy_damage;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].max_energy_damage = t.max_energy_damage;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].regen_damage = t.regen_damage;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].phys_res_dmg = t.phys_res_dmg;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].heat_res_dmg = t.heat_res_dmg;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].energy_res_dmg = t.energy_res_dmg;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].displace = t.displace;
    g_data->d_static[player].weapons[REQ_W5 + tw_count - 1].dmg_type = t.dmg_type;
    return true;
}

u8 load_teleport_module_to_game_data(item_teleport_module t, i32 player) {
    g_data->d_dynamic[player].weapon_uses[REQ_T0] = t.uses;

    g_data->d_static[player].weapons[REQ_T0].min_range = 0;
    g_data->d_static[player].weapons[REQ_T0].max_range= 9;
    g_data->d_static[player].weapons[REQ_T0].damage = t.damage;
    g_data->d_static[player].weapons[REQ_T0].heat_cost = t.heat_cost;
    g_data->d_static[player].weapons[REQ_T0].heat_damage = t.heat_damage;
    g_data->d_static[player].weapons[REQ_T0].max_heat_damage = t.max_heat_damage;
    g_data->d_static[player].weapons[REQ_T0].cooling_damage = t.cooling_damage;
    g_data->d_static[player].weapons[REQ_T0].energy_cost = t.energy_cost;
    g_data->d_static[player].weapons[REQ_T0].energy_damage = t.energy_damage;
    g_data->d_static[player].weapons[REQ_T0].max_energy_damage = t.max_energy_damage;
    g_data->d_static[player].weapons[REQ_T0].regen_damage = t.regen_damage;
    g_data->d_static[player].weapons[REQ_T0].phys_res_dmg = t.phys_res_dmg;
    g_data->d_static[player].weapons[REQ_T0].heat_res_dmg = t.heat_res_dmg;
    g_data->d_static[player].weapons[REQ_T0].energy_res_dmg = t.energy_res_dmg;
    g_data->d_static[player].weapons[REQ_T0].dmg_type = t.dmg_type;
    g_data->d_static[player].weapons[REQ_T0].displace = DISPLACE_TELEPORT;
    return true;
}

u8 load_charge_module_to_game_data(item_charge_module t, i32 player) {
    g_data->d_dynamic[player].weapon_uses[REQ_CH] = t.uses;

    g_data->d_static[player].weapons[REQ_CH].min_range = 0;
    g_data->d_static[player].weapons[REQ_CH].max_range= 9;
    g_data->d_static[player].weapons[REQ_CH].damage = t.damage;
    g_data->d_static[player].weapons[REQ_CH].heat_cost = t.heat_cost;
    g_data->d_static[player].weapons[REQ_CH].heat_damage = t.heat_damage;
    g_data->d_static[player].weapons[REQ_CH].max_heat_damage = t.max_heat_damage;
    g_data->d_static[player].weapons[REQ_CH].cooling_damage = t.cooling_damage;
    g_data->d_static[player].weapons[REQ_CH].energy_cost = t.energy_cost;
    g_data->d_static[player].weapons[REQ_CH].energy_damage = t.energy_damage;
    g_data->d_static[player].weapons[REQ_CH].max_energy_damage = t.max_energy_damage;
    g_data->d_static[player].weapons[REQ_CH].regen_damage = t.regen_damage;
    g_data->d_static[player].weapons[REQ_CH].phys_res_dmg = t.phys_res_dmg;
    g_data->d_static[player].weapons[REQ_CH].heat_res_dmg = t.heat_res_dmg;
    g_data->d_static[player].weapons[REQ_CH].energy_res_dmg = t.energy_res_dmg;
    g_data->d_static[player].weapons[REQ_CH].dmg_type = t.dmg_type;
    g_data->d_static[player].weapons[REQ_CH].displace = DISPLACE_CHARGE;
    return true;
}

u8 load_grapple_module_to_game_data(item_grapple_module t, i32 player) {
    g_data->d_dynamic[player].weapon_uses[REQ_G] = t.uses;

    g_data->d_static[player].weapons[REQ_G].min_range = 0;
    g_data->d_static[player].weapons[REQ_G].max_range= 9;
    g_data->d_static[player].weapons[REQ_G].damage = t.damage;
    g_data->d_static[player].weapons[REQ_G].heat_cost = t.heat_cost;
    g_data->d_static[player].weapons[REQ_G].heat_damage = t.heat_damage;
    g_data->d_static[player].weapons[REQ_G].max_heat_damage = t.max_heat_damage;
    g_data->d_static[player].weapons[REQ_G].cooling_damage = t.cooling_damage;
    g_data->d_static[player].weapons[REQ_G].energy_cost = t.energy_cost;
    g_data->d_static[player].weapons[REQ_G].energy_damage = t.energy_damage;
    g_data->d_static[player].weapons[REQ_G].max_energy_damage = t.max_energy_damage;
    g_data->d_static[player].weapons[REQ_G].regen_damage = t.regen_damage;
    g_data->d_static[player].weapons[REQ_G].phys_res_dmg = t.phys_res_dmg;
    g_data->d_static[player].weapons[REQ_G].heat_res_dmg = t.heat_res_dmg;
    g_data->d_static[player].weapons[REQ_G].energy_res_dmg = t.energy_res_dmg;
    g_data->d_static[player].weapons[REQ_G].dmg_type = t.dmg_type;
    g_data->d_static[player].weapons[REQ_G].displace = DISPLACE_GRAPPLE;
    return true;
}

u8 load_drone_to_game_data(item_drone t, i32 player) {
    g_data->d_static[player].weapons[REQ_T0 + 1].min_range = 0;
    g_data->d_static[player].weapons[REQ_T0 + 1].max_range= 9;
    g_data->d_static[player].weapons[REQ_T0 + 1].damage = t.damage;
    g_data->d_static[player].weapons[REQ_T0 + 1].heat_cost = t.heat_cost;
    g_data->d_static[player].weapons[REQ_T0 + 1].heat_damage = t.heat_damage;
    g_data->d_static[player].weapons[REQ_T0 + 1].max_heat_damage = t.max_heat_damage;
    g_data->d_static[player].weapons[REQ_T0 + 1].cooling_damage = t.cooling_damage;
    g_data->d_static[player].weapons[REQ_T0 + 1].energy_cost = t.energy_cost;
    g_data->d_static[player].weapons[REQ_T0 + 1].energy_damage = t.energy_damage;
    g_data->d_static[player].weapons[REQ_T0 + 1].max_energy_damage = t.max_energy_damage;
    g_data->d_static[player].weapons[REQ_T0 + 1].regen_damage = t.regen_damage;
    g_data->d_static[player].weapons[REQ_T0 + 1].phys_res_dmg = t.phys_res_dmg;
    g_data->d_static[player].weapons[REQ_T0 + 1].heat_res_dmg = t.heat_res_dmg;
    g_data->d_static[player].weapons[REQ_T0 + 1].energy_res_dmg = t.energy_res_dmg;
    g_data->d_static[player].weapons[REQ_T0 + 1].dmg_type = t.dmg_type;
    return true;
}

u8 load_module_to_game_data(item_module t, i32 player, i32 m_count) {
    g_data->d_static[player].max_hp += t.hp;

    g_data->d_dynamic[player].hp += t.hp;
    g_data->d_dynamic[player].max_heat += t.heat;
    g_data->d_dynamic[player].cooling += t.cooling;
    g_data->d_dynamic[player].energy += t.energy;
    g_data->d_dynamic[player].max_energy += t.energy;
    g_data->d_dynamic[player].energy_regen += t.energy_regen;
    g_data->d_dynamic[player].phys_res += t.phys_res;
    g_data->d_dynamic[player].heat_res += t.heat_res;
    g_data->d_dynamic[player].energy_res += t.energy_res;
    return true;
}

u8 load_item_to_game_data(i32 item_type, i32 item_id, i32* total_weight, i32 player, i32 sw_count, i32 tw_count, i32 m_count) {
    switch(item_type) {
        case I_TORSO:
            item_torso t_temp;
            t_temp = get_torso(item_id);
            (*total_weight)+=t_temp.weight;
            load_torso_to_game_data(t_temp, player);
            break;
        case I_LEG:
            item_leg l_temp;
            l_temp = get_leg(item_id);
            (*total_weight)+=l_temp.weight;
            load_leg_to_game_data(l_temp, player);
            break;
        case I_SIDE_WEAPON:
            item_side_weapon sw_temp;
            sw_temp = get_side_weapon(item_id);
            (*total_weight)+=sw_temp.weight;
            load_side_weapon_to_game_data(sw_temp, player, sw_count);
            break;
        case I_TOP_WEAPON:
            item_top_weapon tw_temp;
            tw_temp = get_top_weapon(item_id);
            (*total_weight)+=tw_temp.weight;
            load_top_weapon_to_game_data(tw_temp, player, tw_count);
            break;
        case I_TELEPORT_MODULE:
            item_teleport_module tm_temp;
            tm_temp = get_teleport_module(item_id);
            (*total_weight)+=tm_temp.weight;
            load_teleport_module_to_game_data(tm_temp, player);
            break;
        case I_CHARGE_MODULE:
            item_charge_module cm_temp;
            cm_temp = get_charge_module(item_id);
            (*total_weight)+=cm_temp.weight;
            load_charge_module_to_game_data(cm_temp, player);
            break;
        case I_GRAPPLE_MODULE:
            item_grapple_module gm_temp;
            gm_temp = get_grapple_module(item_id);
            (*total_weight)+=gm_temp.weight;
            load_grapple_module_to_game_data(gm_temp, player);
            break;
        case I_DRONE:
            item_drone d_temp;
            d_temp = get_drone(item_id);
            (*total_weight)+=d_temp.weight;
            load_drone_to_game_data(d_temp, player);
            break;
        case I_MODULE:
            item_module m_temp;
            m_temp = get_module(item_id);
            (*total_weight)+=m_temp.weight;
            load_module_to_game_data(m_temp, player, m_count);
            break;
    }
    if((*total_weight) > 2000)
        return false;
    return true;
}

void check_items(char* items_list, char* result, i32 player) {
    char item_str_buf[MAX_MESSAGE_SIZE];
    char item_list_buffer[MAX_MESSAGE_SIZE];
    char *item_str, *id_str, *type_str, *saveptr1, *saveptr2; 

    i32 item_id;
    i32 item_type;

    i32 t_count = 0;
    i32 l_count = 0;
    i32 tw_count = 0;
    i32 sw_count = 0;
    i32 tm_count = 0;
    i32 cm_count = 0;
    i32 gm_count = 0;
    i32 d_count = 0;
    i32 m_count = 0;
    i32 total_weight = 0;

    strcpy(item_list_buffer, items_list);

    item_str = strtok_r(item_list_buffer, ",", &saveptr1);
    while(item_str != NULL) {

        strcpy(item_str_buf, item_str);

        type_str = strtok_r(item_str_buf, "-", &saveptr2);
        id_str = strtok_r(NULL, "-", &saveptr2);

        if(strcmp(type_str, "T") == 0) {
            if(t_count >= 1) {
                snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (cannot have more than 1 torsos)", item_str);
                return;
            }
            item_type = I_TORSO;
            t_count++;
        }
        else if(strcmp(type_str, "L") == 0) {
            if(l_count >= 1) {
                snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (cannot have more than 1 legs)", item_str);
                return;
            }
            item_type = I_LEG;
            l_count++;
        }
        else if(strcmp(type_str, "SW") == 0) {
            if(sw_count >= 4) {
                snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (cannot have more than 4 side weapons)", item_str);
                return;
            }
            item_type = I_SIDE_WEAPON;
            sw_count++;
        }
        else if(strcmp(type_str, "TW") == 0) {
            if(tw_count >= 2) {
                snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (cannot have more than 2 top weapons)", item_str);
                return;
            }
            item_type = I_TOP_WEAPON;
            tw_count++;
        }
        else if(strcmp(type_str, "TM") == 0) {
            if(tm_count >= 1) {
                snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (cannot have more than 1 teleporters)", item_str);
                return;
            }
            item_type = I_TELEPORT_MODULE;
            tm_count++;
        }
        else if(strcmp(type_str, "CM") == 0) {
            if(cm_count >= 1) {
                snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (cannot have more than 1 charge modules)", item_str);
                return;
            }
            item_type = I_CHARGE_MODULE;
            cm_count++;
        }
        else if(strcmp(type_str, "GM") == 0) {
            if(gm_count >= 1) {
                snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (cannot have more than 1 grapple modules)", item_str);
                return;
            }
            item_type = I_GRAPPLE_MODULE;
            gm_count++;
        }
        else if(strcmp(type_str, "D") == 0) {
            if(d_count >= 1) {
                snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (cannot have more than 1 drones)", item_str);
                return;
            }
            item_type = I_DRONE;
            d_count++;
        }
        else if(strcmp(type_str, "M") == 0) {
            if(m_count >= 8) {
                snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (cannot have more than 8 modules)", item_str);
                return;
            }
            item_type = I_MODULE;
            m_count++;
        }
        else {
            snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (invalid item type: %s)", item_str, type_str);
            return;
        }
        

        if(!is_number(id_str)) {
            snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (invalid item id: %s)", item_str, id_str);
            return;
        }
        item_id = atoi(id_str);
        
        if(!load_item_to_game_data(item_type, item_id, &total_weight, player, sw_count, tw_count, m_count)) {
            snprintf(result, MAX_MESSAGE_SIZE, "invalid item: %s (overweight mech)", item_str);
            return;
        }
        LTRACE("loaded item: %s-%s", type_str, id_str);
        item_str = strtok_r(NULL, ",", &saveptr1);
    }

    strcpy(result, "success");
}

u8 load_game_data(char* items_list_player, char* items_list_opp, i32 pos1, i32 pos2, i32 turn) {
    char result[MAX_MESSAGE_SIZE];

    check_items(items_list_player, result, P1);
    if(strcmp(result, "success") != 0) {
        send_msg("x");
        LERROR("%s", result);
        return false;
    }
    LINFO("loaded player items to game data");

    check_items(items_list_opp, result, P2);
    if(strcmp(result, "success") != 0) {
        send_msg("x");
        LERROR("%s", result);
        return false;
    }
    LINFO("loaded opponent items to game data");
    g_data->actions = 1;
    g_data->d_dynamic[P1].pos = pos1;
    g_data->d_dynamic[P2].pos = pos2;
    g_data->turn = turn;
    return true;
}

i8 process_command(const char* cmd) {


    SM_ASSERT(g_data != 0);

    u32 player = g_data->turn;
    u32 enemy;
    i32 pos[2];
    int command= 0;

    if(player == P1) { enemy = P2; }
    else { enemy = P1; }
    pos[P1] = g_data->d_dynamic[P1].pos; 
    pos[P2] = g_data->d_dynamic[P2].pos; 

    while(command < NUM_REQ_STRINGS) {
        if(strcmp(cmd, cmd_strings[command]) == 0)
            break;
        command++;
    }

    i32 move_dist;
    i32 dist = pos[player] - pos[enemy];

    if(command != REQ_C) {
        SM_ASSERT(g_data->d_dynamic[player].heat <= g_data->d_dynamic[player].max_heat);
    }

    //SM_ASSERT_MSG(command < NUM_REQ_STRINGS, cmd); //crash if invalid client command 
    if(command >= NUM_REQ_STRINGS) { return -1; }
    switch(command) {
        //w1-w6 'command' value matches index into weapons array in game_data
        case REQ_W1: case REQ_W2: case REQ_W3: case REQ_W4: case REQ_W5: case REQ_W6: case REQ_S:
        case REQ_G:  case REQ_CH: case REQ_T0: case REQ_T1: case REQ_T2: case REQ_T3: case REQ_T4: 
        case REQ_T5: case REQ_T6: case REQ_T7: case REQ_T8: case REQ_T9:
            u32 w_index = command;
            if(w_index >= REQ_T0 && w_index <= REQ_T9) { w_index = REQ_T0; }
            LDEBUG("command: %d, w_index: %d, uses: %d", command, w_index, g_data->d_dynamic[player].weapon_uses[w_index]);
            SM_ASSERT_MSG(g_data->d_dynamic[player].weapon_uses[w_index] != 0, "received command to use weapon that has no remaining uses"); //uses check

            //range check 
            if(dist < 0) { dist = -dist; }
            SM_ASSERT_MSG(g_data->d_static[player].weapons[w_index].min_range <= dist && g_data->d_static[player].weapons[w_index].max_range >= dist, 
                        "received command to use weapon that is out of range");

            //cost check
            SM_ASSERT(g_data->d_dynamic[player].energy >= g_data->d_static[player].weapons[w_index].energy_cost);             

            //process damage based on damage type and enemy resistance
            i32 dmg;
            switch(g_data->d_static[player].weapons[w_index].dmg_type) {
                case PHYS: 
                    dmg = g_data->d_static[player].weapons[w_index].damage - g_data->d_dynamic[enemy].phys_res;
                    if(dmg < 0) { dmg = 0; }
                    g_data->d_dynamic[enemy].hp -= dmg;
                    break;
                case HEAT:
                    dmg = g_data->d_static[player].weapons[w_index].damage - g_data->d_dynamic[enemy].heat_res;
                    if(dmg < 0) { dmg = 0; }
                    g_data->d_dynamic[enemy].hp -= dmg;
                    break;
                case ENERGY:
                    dmg = g_data->d_static[player].weapons[w_index].damage - g_data->d_dynamic[enemy].energy_res;
                    if(dmg < 0) { dmg = 0; }
                    g_data->d_dynamic[enemy].hp -= dmg;
                    break;
            }
            
            //apply weapon effects to enemy
            g_data->d_dynamic[player].heat        += g_data->d_static[player].weapons[w_index].heat_cost;
            g_data->d_dynamic[player].energy      -= g_data->d_static[player].weapons[w_index].energy_cost;
            g_data->d_dynamic[enemy].heat         += g_data->d_static[player].weapons[w_index].heat_damage;
            g_data->d_dynamic[enemy].max_heat     -= g_data->d_static[player].weapons[w_index].max_heat_damage;
            g_data->d_dynamic[enemy].cooling      -= g_data->d_static[player].weapons[w_index].cooling_damage;
            g_data->d_dynamic[enemy].energy       -= g_data->d_static[player].weapons[w_index].energy_damage;
            g_data->d_dynamic[enemy].max_energy   -= g_data->d_static[player].weapons[w_index].max_energy_damage;
            g_data->d_dynamic[enemy].energy_regen -= g_data->d_static[player].weapons[w_index].regen_damage;
            g_data->d_dynamic[enemy].phys_res     -= g_data->d_static[player].weapons[w_index].phys_res_dmg;
            g_data->d_dynamic[enemy].heat_res     -= g_data->d_static[player].weapons[w_index].heat_res_dmg;
            g_data->d_dynamic[enemy].energy_res   -= g_data->d_static[player].weapons[w_index].energy_res_dmg;

            //decrease weapon's remaining uses (NOTE: if a weapon's uses is -1, this means it has infinite uses
            if(g_data->d_dynamic[player].weapon_uses[w_index] != -1) { g_data->d_dynamic[player].weapon_uses[w_index]--; }
            
            //process displacement
            i32 enemy_pos = g_data->d_dynamic[enemy].pos;
            dist = pos[player] - pos[enemy];
            switch(g_data->d_static[player].weapons[w_index].displace) {
                case DISPLACE_NONE: break; //do nothing
                case DISPLACE_KNOCKBACK:
                    //knock enemy back 1 space unless @ edge
                    if(g_data->d_dynamic[enemy].pos == 0 || g_data->d_dynamic[enemy].pos == 9) {} //if true, do nothing 
                    else {
                        if(dist < 0) { g_data->d_dynamic[enemy].pos++; } //enemy is to player's right
                        if(dist > 0) { g_data->d_dynamic[enemy].pos--; } //enemy is to player's left
                    }
                    break;
                case DISPLACE_KNOCKFORWARD:
                    //knock enemy forward unless next to player
                    if(dist == 1 || dist == -1) {} //do nothing
                    else {
                        if(dist < 0) { g_data->d_dynamic[enemy].pos--; } //enemy is to player's right
                        if(dist > 0) { g_data->d_dynamic[enemy].pos++; } //enemy is to player's left
                    }
                    break;
                case DISPLACE_GRAPPLE:
                    // move player to position directly adjacent to enemy's position
                    SM_ASSERT(command == REQ_G);
                    if(dist < 0) { g_data->d_dynamic[player].pos = enemy_pos-1; } //enemy is to player's right
                    if(dist > 0) { g_data->d_dynamic[player].pos = enemy_pos+1; } //enemy is to player's left
                    break;
                case DISPLACE_CHARGE:
                    //move player to position directly adjacent to enemy's position. knock enemy back unless @ edge
                    SM_ASSERT(command == REQ_CH);
                    if(dist < 0) { 
                        g_data->d_dynamic[player].pos = enemy_pos-1; 
                        if(enemy_pos < 9)
                            g_data->d_dynamic[enemy].pos+=1;
                    } //enemy is to player's right
                    if(dist > 0) { 
                        g_data->d_dynamic[player].pos = enemy_pos+1; 
                        if(enemy_pos > 0)
                            g_data->d_dynamic[enemy].pos-=1;
                    } //enemy is to player's left
                    break;
                case DISPLACE_TELEPORT:
                    //move player to position indicated by number after 'T' in command (i.e. req_T3 goes to pos 3) ASSERT THAT THIS POSITION IS NOT ENEMY'S POSITION
                    SM_ASSERT(command >= REQ_T0 && command <= REQ_T9);
                    i32 target_pos = command - REQ_T0;
                    SM_ASSERT(target_pos != enemy_pos);
                    g_data->d_dynamic[player].pos = target_pos;
                    break;
            }
            LDEBUG("player %d used weapon/ability %d", player, w_index);
            break;
        case REQ_DA:
            SM_ASSERT(g_data->d_dynamic[player].drone_active == false);
            g_data->d_dynamic[player].drone_active = true;
            break;
        case REQ_DD:
            SM_ASSERT(g_data->d_dynamic[player].drone_active == true);
            g_data->d_dynamic[player].drone_active = false;
            break;
        case REQ_C:
            g_data->d_dynamic[player].heat -= g_data->d_dynamic[player].cooling;
            if(g_data->d_dynamic[player].heat < 0) {
                g_data->d_dynamic[player].heat = 0;
            }
            break;
        case REQ_R1:
        case REQ_R2:
        case REQ_R3:
            move_dist = command - REQ_R1 + 1;
            SM_ASSERT(g_data->d_dynamic[player].pos + move_dist <= 9);
            SM_ASSERT(g_data->d_dynamic[player].pos + move_dist != g_data->d_dynamic[enemy].pos);
            g_data->d_dynamic[player].pos += move_dist;
            break;
        case REQ_L1:
        case REQ_L2:
        case REQ_L3:
            move_dist = command - REQ_L1 + 1; 
            SM_ASSERT(g_data->d_dynamic[player].pos - move_dist >= 0);
            SM_ASSERT(g_data->d_dynamic[player].pos - move_dist != g_data->d_dynamic[enemy].pos);
            g_data->d_dynamic[player].pos -= move_dist;
            break;
        case REQ_ER:
            SM_ASSERT(command != REQ_ER); //this will always assert if it is reached 
            break;
        default:
            //TODO: notfiy clients that we are going to crash (i.e. send them 'er')
            SM_ASSERT_MSG(0, "invalid command received from client");
            break;
    }

    SM_ASSERT(g_data->actions > 0 && g_data->actions < 3);
    g_data->actions--;
    if(g_data->actions == 0) {
        if(g_data->d_dynamic[player].drone_active) {

            //process damage based on damage type and enemy resistance
            i32 dmg;
            switch(g_data->d_static[player].weapons[10].dmg_type) {
                case PHYS: 
                    dmg = g_data->d_static[player].weapons[10].damage - g_data->d_dynamic[enemy].phys_res;
                    if(dmg < 0) { dmg = 0; }
                    g_data->d_dynamic[enemy].hp -= dmg;
                    break;
                case HEAT:
                    dmg = g_data->d_static[player].weapons[10].damage - g_data->d_dynamic[enemy].heat_res;
                    if(dmg < 0) { dmg = 0; }
                    g_data->d_dynamic[enemy].hp -= dmg;
                    break;
                case ENERGY:
                    dmg = g_data->d_static[player].weapons[10].damage - g_data->d_dynamic[enemy].energy_res;
                    if(dmg < 0) { dmg = 0; }
                    g_data->d_dynamic[enemy].hp -= dmg;
                    break;
            }
            
            //apply drone attack effects to enemy
            g_data->d_dynamic[player].heat        += g_data->d_static[player].weapons[10].heat_cost;
            g_data->d_dynamic[player].energy      -= g_data->d_static[player].weapons[10].energy_cost;
            g_data->d_dynamic[enemy].heat         += g_data->d_static[player].weapons[10].heat_damage;
            g_data->d_dynamic[enemy].max_heat     -= g_data->d_static[player].weapons[10].max_heat_damage;
            g_data->d_dynamic[enemy].cooling      -= g_data->d_static[player].weapons[10].cooling_damage;
            g_data->d_dynamic[enemy].energy       -= g_data->d_static[player].weapons[10].energy_damage;
            g_data->d_dynamic[enemy].max_energy   -= g_data->d_static[player].weapons[10].max_energy_damage;
            g_data->d_dynamic[enemy].energy_regen -= g_data->d_static[player].weapons[10].regen_damage;
            g_data->d_dynamic[enemy].phys_res     -= g_data->d_static[player].weapons[10].phys_res_dmg;
            g_data->d_dynamic[enemy].heat_res     -= g_data->d_static[player].weapons[10].heat_res_dmg;
            g_data->d_dynamic[enemy].energy_res   -= g_data->d_static[player].weapons[10].energy_res_dmg;
            LTRACE("player %d's drone attacked", player);
        }
        SM_ASSERT(g_data->turn == player);
        g_data->d_dynamic[player].energy += g_data->d_dynamic[player].energy_regen;
        g_data->turn = enemy;
        LTRACE("switched turn to player %d", g_data->turn);
        g_data->actions = 2;
    }
    if(g_data->d_dynamic[enemy].hp <= 0) { return player; } //return 1 for P1 win and 2 for P2 win

    LDEBUG("p1 hp: %d\tp2 hp: %d\n\t p1 pos: %d\tp2 pos: %d\n\t p1 heat: %d\tp2 heat: %d\n\t p1 nrg: %d\tp2 nrg: %d", g_data->d_dynamic[P1].hp, g_data->d_dynamic[P2].hp, g_data->d_dynamic[P1].pos, g_data->d_dynamic[P2].pos, g_data->d_dynamic[P1].heat, g_data->d_dynamic[P2].heat, g_data->d_dynamic[P1].energy, g_data->d_dynamic[P2].energy);//TODO: temp

    return 0;
}
