#include "game_data.h"
#include "asserts.h"

#include "logger.h"

#include <string.h>
#include <stdlib.h>

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

i8 init_game_data() {
    _ASSERT(g_data == 0);
    g_data = (game_data*)malloc(sizeof(game_data));
    memset(g_data, 0, sizeof(game_data)); 
    LINFO("initialized game data.");
    return true;
}

void free_game_data() {
    _ASSERT(g_data != 0);
    free(g_data);
    g_data = 0;
    LINFO("uninitialized game data.");
}

u32 get_turn() {
    return g_data->turn;
}

i8 process_command(const char* cmd) {

    _ASSERT(g_data != 0);

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
    i32 dist = pos[P1] - pos[P2];
    _ASSERT_MSG(command < NUM_REQ_STRINGS, cmd); //crash if invalid client command 
    switch(command) {
        //w1-w6 'command' value matches index into weapons array in game_data
        case REQ_W1: case REQ_W2: case REQ_W3: case REQ_W4: case REQ_W5: case REQ_W6: case REQ_S:
        case REQ_G:  case REQ_CH: case REQ_T0: case REQ_T1: case REQ_T2: case REQ_T3: case REQ_T4: 
        case REQ_T5: case REQ_T6: case REQ_T7: case REQ_T8: case REQ_T9:
            u32 w_index = command;
            if(w_index >= REQ_T0 || w_index <= REQ_T9) { w_index = REQ_T0; }
            _ASSERT_MSG(g_data->d_dynamic[player].weapon_uses[w_index] > 0, "received command to use weapon that has no remaining uses"); //uses check

            //range check 
            if(dist < 0) { dist = -dist; }
            _ASSERT_MSG(g_data->d_static[player].weapons[w_index].min_range <= dist && g_data->d_static[player].weapons[w_index].max_range >= dist, 
                        "received command to use weapon that is out of range");
            
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
                    _ASSERT(command == REQ_G);
                    if(dist < 0) { g_data->d_dynamic[player].pos = enemy_pos-1; } //enemy is to player's right
                    if(dist > 0) { g_data->d_dynamic[player].pos = enemy_pos+1; } //enemy is to player's left
                    break;
                case DISPLACE_CHARGE:
                    //move player to position directly adjacent to enemy's position. knock enemy back unless @ edge
                    _ASSERT(command == REQ_CH);
                    if(dist < 0) { 
                        g_data->d_dynamic[player].pos = enemy_pos-1; 
                        if(enemy_pos < 9)
                            g_data->d_dynamic[enemy].pos++;
                    } //enemy is to player's right
                    if(dist > 0) { 
                        g_data->d_dynamic[player].pos = enemy_pos+1; 
                        if(enemy_pos > 0)
                            g_data->d_dynamic[enemy].pos--;
                    } //enemy is to player's left
                    break;
                case DISPLACE_TELEPORT:
                    //move player to position indicated by number after 'T' in command (i.e. req_T3 goes to pos 3) ASSERT THAT THIS POSITION IS NOT ENEMY'S POSITION
                    _ASSERT(command >= REQ_T0 && command <= REQ_T9);
                    i32 target_pos = command - REQ_T0;
                    _ASSERT(target_pos != enemy_pos);
                    g_data->d_dynamic[player].pos = target_pos;
                    break;
            }
            LDEBUG("player %d used weapon/ability %d", player, w_index);
            break;
        case REQ_DA:
            _ASSERT(g_data->d_dynamic[player].drone_active == false);
            g_data->d_dynamic[player].drone_active = true;
            break;
        case REQ_DD:
            _ASSERT(g_data->d_dynamic[player].drone_active == true);
            g_data->d_dynamic[player].drone_active = false;
            break;
        case REQ_C:
            g_data->d_dynamic[player].heat -= g_data->d_dynamic[player].cooling;
            break;
        case REQ_R1:
        case REQ_R2:
        case REQ_R3:
            move_dist = command - REQ_R1 + 1;
            _ASSERT(g_data->d_dynamic[player].pos + move_dist <= 9);
            g_data->d_dynamic[player].pos += move_dist;
            break;
        case REQ_L1:
        case REQ_L2:
        case REQ_L3:
            move_dist = command - REQ_L1 + 1; 
            _ASSERT(g_data->d_dynamic[player].pos - move_dist >= 0);
            g_data->d_dynamic[player].pos -= move_dist;
            break;
        case REQ_ER:
            _ASSERT(command != REQ_ER); //this will always assert if it is reached 
            break;
        default:
            //TODO: notfiy clients that we are going to crash (i.e. send them 'er')
            _ASSERT_MSG(0, "invalid command received from client");
            break;
    }

    _ASSERT(g_data->actions > 0 && g_data->actions < 3);
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
        _ASSERT(g_data->turn != player);
        g_data->turn = enemy;
        LTRACE("switched turn to player %d", g_data->turn);
        g_data->actions = 2;
    }
    if(g_data->d_dynamic[enemy].hp <= 0) { return enemy + 1; } //return 1 for P1 win and 2 for P2 win
    return 0;
}
