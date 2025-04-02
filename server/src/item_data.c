#include "item_data.h"

#include "asserts.h"
#include "defines.h"
#include "files.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

const char* item_str_list[] = {"torsos", "legs", "side_weapons", "top_weapons", "teleport_modules", "charge_modules", 
                               "grapple_modules", "drones", "modules"}; 

static file item_data_file;

static item_torso*             torso_list = 0;
static item_leg*               leg_list = 0;
static item_side_weapon*       side_weapon_list = 0;
static item_top_weapon*        top_weapon_list = 0;
static item_teleport_module*   teleport_module_list = 0;
static item_charge_module*     charge_module_list = 0;
static item_grapple_module*    grapple_module_list = 0;
static item_drone*             drone_list = 0;
static item_module*            module_list = 0;

static u32 item_count_list[I_TYPES_NUM];

u8 init_item_data();

u8 load_torso_data(u32 count);
u8 load_leg_data(u32 count);
u8 load_side_weapon_data(u32 count);
u8 load_top_weapon_data(u32 count);
u8 load_teleport_module_data(u32 count);
u8 load_charge_module_data(u32 count);
u8 load_grapple_module_data(u32 count);
u8 load_drone_data();
u8 load_module_data();

u32 get_item_count(u32 item_type_index) {
    char item_type_str[MAX_LINE_SIZE];
    char line_buffer[MAX_LINE_SIZE];
    char* token;
    strcpy(item_type_str, item_str_list[item_type_index]);
    read_line(item_data_file, line_buffer, MAX_LINE_SIZE);
    token = strtok(line_buffer, ":");
    _ASSERT(strcmp(token, item_type_str) == 0);
    token = strtok(NULL, ":");
    _ASSERT(is_number(token) == true);
    return atoi(token);
}

u8 load_item_data() {
    u8 result;
    char line_buffer[MAX_LINE_SIZE];

    result = open_file("../server/data/item_list.txt", &item_data_file, "r"); 
    _ASSERT(result == true);
    LTRACE("opened item data file.");
       
    //#item_counts
    read_line(item_data_file, line_buffer, MAX_LINE_SIZE);
    result = strcmp(line_buffer, "#item_counts");
    _ASSERT_MSG(result == 0, "first line of item data file must be #item_counts");

    //item counts
    for(int i = 0; i < I_TYPES_NUM; i++) {
        item_count_list[i] = get_item_count(i);
    }
    init_item_data(item_count_list, I_TYPES_NUM);

    //load items
    char* temp_buffer;
    u32 item_str_index;
    while(read_line(item_data_file, line_buffer, MAX_LINE_SIZE)) {
        if(line_buffer[0] == '#') {
            item_str_index = 0;
            temp_buffer = line_buffer + 1; //increment past '#' character
            for(int i = 0; i < I_TYPES_NUM; i++) {
                if(strcmp(temp_buffer, item_str_list[item_str_index]) == 0) { break; }
                item_str_index++;
            }
            LTRACE("reading %s...", line_buffer+1);
            switch(item_str_index) {
                case I_TORSO:
                    load_torso_data(item_count_list[item_str_index]);
                    break;
                case I_LEG:
                    load_leg_data(item_count_list[item_str_index]);
                    break;
                case I_SIDE_WEAPON:
                    load_side_weapon_data(item_count_list[item_str_index]);
                    break;
                case I_TOP_WEAPON:
                    load_top_weapon_data(item_count_list[item_str_index]);
                    break;
                case I_TELEPORT_MODULE:
                    load_teleport_module_data(item_count_list[item_str_index]);
                    break;
                case I_CHARGE_MODULE:
                    load_charge_module_data(item_count_list[item_str_index]);
                    break;
                case I_GRAPPLE_MODULE:
                    load_grapple_module_data(item_count_list[item_str_index]);
                    break;
                case I_DRONE:
                    load_drone_data(item_count_list[item_str_index]);
                    break;
                case I_MODULE:
                    load_module_data(item_count_list[item_str_index]);
                    break;
            }
            LTRACE("finished reading %s.", line_buffer+1);
        }
        //do nothing if we aren't starting a section (i.e. ignore the line)
    }

    result = close_file(&item_data_file);
    LTRACE("closed item data file.");
    LINFO("item data loaded.");
    _ASSERT(result == true);
    LTRACE("memory used for item data: %lu", item_count_list[I_TORSO] * sizeof(item_torso) + item_count_list[I_LEG] 
            * sizeof(item_leg) 
            + item_count_list[I_SIDE_WEAPON] * sizeof(item_side_weapon) + item_count_list[I_TOP_WEAPON] 
            * sizeof(item_top_weapon) + item_count_list[I_TELEPORT_MODULE] * sizeof(item_teleport_module)
            + item_count_list[I_CHARGE_MODULE] * sizeof(item_charge_module) + item_count_list[I_GRAPPLE_MODULE] 
            * sizeof(item_grapple_module) + item_count_list[I_DRONE] * sizeof(item_drone)
            + item_count_list[I_MODULE] * sizeof(item_module)
    );
    return true;
}

u8 init_item_data() {
    _ASSERT(torso_list              == 0);
    _ASSERT(leg_list                == 0);
    _ASSERT(side_weapon_list        == 0);
    _ASSERT(top_weapon_list         == 0);
    _ASSERT(teleport_module_list    == 0);
    _ASSERT(charge_module_list      == 0);
    _ASSERT(grapple_module_list     == 0);
    _ASSERT(drone_list              == 0);
    _ASSERT(module_list             == 0);
    
    torso_list              = (item_torso*)malloc(sizeof(item_torso) * (item_count_list[I_TORSO]));
    leg_list                = (item_leg*)malloc(sizeof(item_leg) * (item_count_list[I_LEG]));
    side_weapon_list        = (item_side_weapon*)malloc(sizeof(item_side_weapon) * (item_count_list[I_SIDE_WEAPON]));
    top_weapon_list         = (item_top_weapon*)malloc(sizeof(item_top_weapon) * (item_count_list[I_TOP_WEAPON]));
    teleport_module_list    = (item_teleport_module*)malloc(sizeof(item_teleport_module) * (item_count_list[I_TELEPORT_MODULE]));
    charge_module_list      = (item_charge_module*)malloc(sizeof(item_charge_module) * (item_count_list[I_CHARGE_MODULE]));
    grapple_module_list     = (item_grapple_module*)malloc(sizeof(item_grapple_module) * (item_count_list[I_GRAPPLE_MODULE]));
    drone_list              = (item_drone*)malloc(sizeof(item_drone) * (item_count_list[I_DRONE]));
    module_list             = (item_module*)malloc(sizeof(item_module) * (item_count_list[I_MODULE]));

    memset(torso_list, 0, sizeof(item_torso) * item_count_list[I_TORSO]);
    memset(leg_list, 0, sizeof(item_leg) * item_count_list[I_LEG]);
    memset(side_weapon_list, 0, sizeof(item_side_weapon) * item_count_list[I_SIDE_WEAPON]);
    memset(top_weapon_list, 0, sizeof(item_top_weapon) * item_count_list[I_TOP_WEAPON]);
    memset(teleport_module_list, 0, sizeof(item_teleport_module) * item_count_list[I_TELEPORT_MODULE]);
    memset(charge_module_list, 0, sizeof(item_charge_module) * item_count_list[I_CHARGE_MODULE]);
    memset(grapple_module_list, 0, sizeof(item_grapple_module) * item_count_list[I_GRAPPLE_MODULE]);
    memset(drone_list, 0, sizeof(item_drone) * item_count_list[I_DRONE]);
    memset(module_list, 0, sizeof(item_module) * item_count_list[I_MODULE]);

    _ASSERT(torso_list              != 0);
    _ASSERT(leg_list                != 0);
    _ASSERT(side_weapon_list        != 0);
    _ASSERT(top_weapon_list         != 0);
    _ASSERT(teleport_module_list    != 0);
    _ASSERT(charge_module_list      != 0);
    _ASSERT(grapple_module_list     != 0);
    _ASSERT(drone_list              != 0);
    _ASSERT(module_list             != 0);

    LINFO("item data initialized");
    return true;
}

u8 load_torso_data(u32 count) {
    char line_buffer[MAX_LINE_SIZE+1];
    char item_name[MAX_LINE_SIZE];
    char* token;
    char attrib[MAX_LINE_SIZE];
    i32 value;
    u32 i_ID;
    u8 first_item_read_started = false;
    u32 index = 0;

    read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1);
    while(strcmp(line_buffer, "#end") != 0) {
        if(line_buffer[0] == '\0') {
            //empty line do nothing
        }
        else if(line_buffer[0] == '\t' || line_buffer[0] == ' ')
        {   //reading item attribute
            index = 0;
            while(line_buffer[index] == ' ' || line_buffer[index] == '\t')
                index++;
            _ASSERT(first_item_read_started == true);
            token = strtok(line_buffer+index, ":");
            strcpy(attrib, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            value = atoi(token);

            if(strcmp(attrib, "weight") == 0) { torso_list[i_ID].weight = value; }
            else if(strcmp(attrib, "hp") == 0) { torso_list[i_ID].hp = value; }
            else if(strcmp(attrib, "heat") == 0) { torso_list[i_ID].heat = value; }
            else if(strcmp(attrib, "energy") == 0) { torso_list[i_ID].energy = value; }
            else if(strcmp(attrib, "phys_res") == 0) { torso_list[i_ID].phys_res = value; }
            else if(strcmp(attrib, "heat_res") == 0) { torso_list[i_ID].heat_res = value; }
            else if(strcmp(attrib, "energy_res") == 0) { torso_list[i_ID].energy_res = value; }
            else if(strcmp(attrib, "cooling") == 0) { torso_list[i_ID].cooling = value; }
            else if(strcmp(attrib, "energy_regen") == 0) { torso_list[i_ID].energy_regen = value; }
            else { _ASSERT_MSG(false, "attempted to load invalid attribute for a torso item"); }
        }
        else { //next item
            token = strtok(line_buffer, ":");
            strcpy(item_name, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            i_ID = atoi(token);
            LTRACE("reading item %s [ID = T-%d]...", item_name, i_ID);
            _ASSERT(i_ID < count);
            if(!first_item_read_started) { first_item_read_started = true; }
        }
        read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1); //read next line b4 next loop
    }
    return true;
}

u8 load_leg_data(u32 count) {
    char line_buffer[MAX_LINE_SIZE+1];
    char item_name[MAX_LINE_SIZE];
    char* token;
    char attrib[MAX_LINE_SIZE];
    i32 value;
    u32 i_ID;
    u8 first_item_read_started = false;
    u32 index = 0;

    read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1);
    while(strcmp(line_buffer, "#end") != 0) {
        if(line_buffer[0] == '\0') {
            //empty line do nothing
        }
        else if(line_buffer[0] == '\t' || line_buffer[0] == ' ')
        {   //reading item attribute
            index = 0;
            while(line_buffer[index] == ' ' || line_buffer[index] == '\t')
                index++;
            _ASSERT(first_item_read_started == true);
            token = strtok(line_buffer+index, ":");
            strcpy(attrib, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            value = atoi(token);

            if(strcmp(attrib, "weight") == 0) { leg_list[i_ID].weight = value; }
            else if(strcmp(attrib, "hp") == 0) { leg_list[i_ID].hp = value; }
            else if(strcmp(attrib, "uses") == 0) { leg_list[i_ID].uses = value; }
            else if(strcmp(attrib, "damage") == 0) { leg_list[i_ID].damage = value; }
            else if(strcmp(attrib, "heat_cost") == 0) { leg_list[i_ID].heat_cost = value; }
            else if(strcmp(attrib, "heat_damage") == 0) { leg_list[i_ID].heat_damage = value; }
            else if(strcmp(attrib, "max_heat_damage") == 0) { leg_list[i_ID].max_heat_damage = value; }
            else if(strcmp(attrib, "cooling_damage") == 0) { leg_list[i_ID].cooling_damage = value; }
            else if(strcmp(attrib, "energy_cost") == 0) { leg_list[i_ID].energy_cost = value; }
            else if(strcmp(attrib, "energy_damage") == 0) { leg_list[i_ID].energy_damage = value; }
            else if(strcmp(attrib, "max_energy_damage") == 0) { leg_list[i_ID].max_energy_damage = value; }
            else if(strcmp(attrib, "regen_damage") == 0) { leg_list[i_ID].regen_damage = value; }
            else if(strcmp(attrib, "phys_res_dmg") == 0) { leg_list[i_ID].phys_res_dmg = value; }
            else if(strcmp(attrib, "heat_res_dmg") == 0) { leg_list[i_ID].heat_res_dmg = value; }
            else if(strcmp(attrib, "energy_res_dmg") == 0) { leg_list[i_ID].energy_res_dmg = value; }
            else if(strcmp(attrib, "displace") == 0) { leg_list[i_ID].displace = value; }
            else if(strcmp(attrib, "dmg_type") == 0) { leg_list[i_ID].dmg_type = value; }
            else { _ASSERT_MSG(false, "attempted to load invalid attribute for a leg item"); }
        }
        else { //next item
            token = strtok(line_buffer, ":");
            strcpy(item_name, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            i_ID = atoi(token);
            LTRACE("reading item %s [ID = L-%d]...", item_name, i_ID);
            _ASSERT(i_ID < count);
            if(!first_item_read_started) { first_item_read_started = true; }
        }
        read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1); //read next line b4 next loop
    }
    return true;
}

u8 load_side_weapon_data(u32 count) {
    char line_buffer[MAX_LINE_SIZE+1];
    char item_name[MAX_LINE_SIZE];
    char* token;
    char attrib[MAX_LINE_SIZE];
    i32 value;
    u32 i_ID;
    u8 first_item_read_started = false;
    u32 index = 0;

    read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1);
    while(strcmp(line_buffer, "#end") != 0) {
        if(line_buffer[0] == '\0') {
            //empty line do nothing
        }
        else if(line_buffer[0] == '\t' || line_buffer[0] == ' ')
        {   //reading item attribute
            index = 0;
            while(line_buffer[index] == ' ' || line_buffer[index] == '\t')
                index++;
            _ASSERT(first_item_read_started == true);
            token = strtok(line_buffer+index, ":");
            strcpy(attrib, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            value = atoi(token);

            if(strcmp(attrib, "weight") == 0)                   { side_weapon_list[i_ID].weight = value; }
            else if(strcmp(attrib, "uses") == 0)                { side_weapon_list[i_ID].uses = value; }
            else if(strcmp(attrib, "min_range") == 0)           { side_weapon_list[i_ID].min_range = value; }
            else if(strcmp(attrib, "max_range") == 0)           { side_weapon_list[i_ID].max_range = value; }
            else if(strcmp(attrib, "damage") == 0)              { side_weapon_list[i_ID].damage = value; }
            else if(strcmp(attrib, "heat_cost") == 0)           { side_weapon_list[i_ID].heat_cost = value; }
            else if(strcmp(attrib, "heat_damage") == 0)         { side_weapon_list[i_ID].heat_damage = value; }
            else if(strcmp(attrib, "max_heat_damage") == 0)     { side_weapon_list[i_ID].max_heat_damage = value; }
            else if(strcmp(attrib, "cooling_damage") == 0)      { side_weapon_list[i_ID].cooling_damage = value; }
            else if(strcmp(attrib, "energy_cost") == 0)         { side_weapon_list[i_ID].energy_cost = value; }
            else if(strcmp(attrib, "energy_damage") == 0)       { side_weapon_list[i_ID].energy_damage = value; }
            else if(strcmp(attrib, "max_energy_damage") == 0)   { side_weapon_list[i_ID].max_energy_damage = value; }
            else if(strcmp(attrib, "regen_damage") == 0)        { side_weapon_list[i_ID].regen_damage = value; }
            else if(strcmp(attrib, "phys_res_dmg") == 0)        { side_weapon_list[i_ID].phys_res_dmg = value; }
            else if(strcmp(attrib, "heat_res_dmg") == 0)        { side_weapon_list[i_ID].heat_res_dmg = value; }
            else if(strcmp(attrib, "energy_res_dmg") == 0)      { side_weapon_list[i_ID].energy_res_dmg = value; }
            else if(strcmp(attrib, "displace") == 0)            { side_weapon_list[i_ID].displace = value; }
            else if(strcmp(attrib, "dmg_type") == 0)            { side_weapon_list[i_ID].dmg_type = value; }
            else { _ASSERT_MSG(false, "attempted to load invalid attribute for a side weapon item"); }
        }
        else { //next item
            token = strtok(line_buffer, ":");
            strcpy(item_name, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            i_ID = atoi(token);
            LTRACE("reading item %s [ID = SW-%d]...", item_name, i_ID);
            _ASSERT(i_ID < count);
            if(!first_item_read_started) { first_item_read_started = true; }
        }
        read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1); //read next line b4 next loop
    }
    return true;
}

u8 load_top_weapon_data(u32 count) {
    char line_buffer[MAX_LINE_SIZE+1];
    char item_name[MAX_LINE_SIZE];
    char* token;
    char attrib[MAX_LINE_SIZE];
    i32 value;
    u32 i_ID;
    u8 first_item_read_started = false;
    u32 index = 0;

    read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1);
    while(strcmp(line_buffer, "#end") != 0) {
        if(line_buffer[0] == '\0') {
            //empty line do nothing
        }
        else if(line_buffer[0] == '\t' || line_buffer[0] == ' ')
        {   //reading item attribute
            index = 0;
            while(line_buffer[index] == ' ' || line_buffer[index] == '\t')
                index++;
            _ASSERT(first_item_read_started == true);
            token = strtok(line_buffer+index, ":");
            strcpy(attrib, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            value = atoi(token);

            if(strcmp(attrib, "weight") == 0)                   { top_weapon_list[i_ID].weight = value; }
            else if(strcmp(attrib, "uses") == 0)                { top_weapon_list[i_ID].uses = value; }
            else if(strcmp(attrib, "min_range") == 0)           { top_weapon_list[i_ID].min_range = value; }
            else if(strcmp(attrib, "max_range") == 0)           { top_weapon_list[i_ID].max_range = value; }
            else if(strcmp(attrib, "damage") == 0)              { top_weapon_list[i_ID].damage = value; }
            else if(strcmp(attrib, "heat_cost") == 0)           { top_weapon_list[i_ID].heat_cost = value; }
            else if(strcmp(attrib, "heat_damage") == 0)         { top_weapon_list[i_ID].heat_damage = value; }
            else if(strcmp(attrib, "max_heat_damage") == 0)     { top_weapon_list[i_ID].max_heat_damage = value; }
            else if(strcmp(attrib, "cooling_damage") == 0)      { top_weapon_list[i_ID].cooling_damage = value; }
            else if(strcmp(attrib, "energy_cost") == 0)         { top_weapon_list[i_ID].energy_cost = value; }
            else if(strcmp(attrib, "energy_damage") == 0)       { top_weapon_list[i_ID].energy_damage = value; }
            else if(strcmp(attrib, "max_energy_damage") == 0)   { top_weapon_list[i_ID].max_energy_damage = value; }
            else if(strcmp(attrib, "regen_damage") == 0)        { top_weapon_list[i_ID].regen_damage = value; }
            else if(strcmp(attrib, "phys_res_dmg") == 0)        { top_weapon_list[i_ID].phys_res_dmg = value; }
            else if(strcmp(attrib, "heat_res_dmg") == 0)        { top_weapon_list[i_ID].heat_res_dmg = value; }
            else if(strcmp(attrib, "energy_res_dmg") == 0)      { top_weapon_list[i_ID].energy_res_dmg = value; }
            else if(strcmp(attrib, "displace") == 0)            { top_weapon_list[i_ID].displace = value; }
            else if(strcmp(attrib, "dmg_type") == 0)            { top_weapon_list[i_ID].dmg_type = value; }
            else { _ASSERT_MSG(false, "attempted to load invalid attribute for a top weapon item"); }
        }
        else { //next item
            token = strtok(line_buffer, ":");
            strcpy(item_name, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            i_ID = atoi(token);
            LTRACE("reading item %s [ID = TW-%d]...", item_name, i_ID);
            _ASSERT(i_ID < count);
            if(!first_item_read_started) { first_item_read_started = true; }
        }
        read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1); //read next line b4 next loop
    }
    return true;
}

u8 load_teleport_module_data(u32 count) {
    char line_buffer[MAX_LINE_SIZE+1];
    char item_name[MAX_LINE_SIZE];
    char* token;
    char attrib[MAX_LINE_SIZE];
    i32 value;
    u32 i_ID;
    u8 first_item_read_started = false;
    u32 index = 0;

    read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1);
    while(strcmp(line_buffer, "#end") != 0) {
        if(line_buffer[0] == '\0') {
            //empty line do nothing
        }
        else if(line_buffer[0] == '\t' || line_buffer[0] == ' ')
        {   //reading item attribute
            index = 0;
            while(line_buffer[index] == ' ' || line_buffer[index] == '\t')
                index++;
            _ASSERT(first_item_read_started == true);
            token = strtok(line_buffer+index, ":");
            strcpy(attrib, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            value = atoi(token);

            if(strcmp(attrib, "weight") == 0)                   { teleport_module_list[i_ID].weight = value; }
            else if(strcmp(attrib, "damage") == 0)              { teleport_module_list[i_ID].damage = value; }
            else if(strcmp(attrib, "heat_cost") == 0)           { teleport_module_list[i_ID].heat_cost = value; }
            else if(strcmp(attrib, "heat_damage") == 0)         { teleport_module_list[i_ID].heat_damage = value; }
            else if(strcmp(attrib, "max_heat_damage") == 0)     { teleport_module_list[i_ID].max_heat_damage = value; }
            else if(strcmp(attrib, "cooling_damage") == 0)      { teleport_module_list[i_ID].cooling_damage = value; }
            else if(strcmp(attrib, "energy_cost") == 0)         { teleport_module_list[i_ID].energy_cost = value; }
            else if(strcmp(attrib, "energy_damage") == 0)       { teleport_module_list[i_ID].energy_damage = value; }
            else if(strcmp(attrib, "max_energy_damage") == 0)   { teleport_module_list[i_ID].max_energy_damage = value; }
            else if(strcmp(attrib, "regen_damage") == 0)        { teleport_module_list[i_ID].regen_damage = value; }
            else if(strcmp(attrib, "phys_res_dmg") == 0)        { teleport_module_list[i_ID].phys_res_dmg = value; }
            else if(strcmp(attrib, "heat_res_dmg") == 0)        { teleport_module_list[i_ID].heat_res_dmg = value; }
            else if(strcmp(attrib, "energy_res_dmg") == 0)      { teleport_module_list[i_ID].energy_res_dmg = value; }
            else if(strcmp(attrib, "dmg_type") == 0)            { teleport_module_list[i_ID].dmg_type = value; }
            else { _ASSERT_MSG(false, "attempted to load invalid attribute for a teleport module item"); }
        }
        else { //next item
            token = strtok(line_buffer, ":");
            strcpy(item_name, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            i_ID = atoi(token);
            LTRACE("reading item %s [ID = T-%d]...", item_name, i_ID);
            _ASSERT(i_ID < count);
            if(!first_item_read_started) { first_item_read_started = true; }
        }
        read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1); //read next line b4 next loop
    }
    return true;
}

u8 load_charge_module_data(u32 count) {
    char line_buffer[MAX_LINE_SIZE+1];
    char item_name[MAX_LINE_SIZE];
    char* token;
    char attrib[MAX_LINE_SIZE];
    i32 value;
    u32 i_ID;
    u8 first_item_read_started = false;
    u32 index = 0;

    read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1);
    while(strcmp(line_buffer, "#end") != 0) {
        if(line_buffer[0] == '\0') {
            //empty line do nothing
        }
        else if(line_buffer[0] == '\t' || line_buffer[0] == ' ')
        {   //reading item attribute
            index = 0;
            while(line_buffer[index] == ' ' || line_buffer[index] == '\t')
                index++;
            _ASSERT(first_item_read_started == true);
            token = strtok(line_buffer+index, ":");
            strcpy(attrib, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            value = atoi(token);

            if(strcmp(attrib, "weight") == 0)                   { charge_module_list[i_ID].weight = value; }
            else if(strcmp(attrib, "damage") == 0)              { charge_module_list[i_ID].damage = value; }
            else if(strcmp(attrib, "heat_cost") == 0)           { charge_module_list[i_ID].heat_cost = value; }
            else if(strcmp(attrib, "heat_damage") == 0)         { charge_module_list[i_ID].heat_damage = value; }
            else if(strcmp(attrib, "max_heat_damage") == 0)     { charge_module_list[i_ID].max_heat_damage = value; }
            else if(strcmp(attrib, "cooling_damage") == 0)      { charge_module_list[i_ID].cooling_damage = value; }
            else if(strcmp(attrib, "energy_cost") == 0)         { charge_module_list[i_ID].energy_cost = value; }
            else if(strcmp(attrib, "energy_damage") == 0)       { charge_module_list[i_ID].energy_damage = value; }
            else if(strcmp(attrib, "max_energy_damage") == 0)   { charge_module_list[i_ID].max_energy_damage = value; }
            else if(strcmp(attrib, "regen_damage") == 0)        { charge_module_list[i_ID].regen_damage = value; }
            else if(strcmp(attrib, "phys_res_dmg") == 0)        { charge_module_list[i_ID].phys_res_dmg = value; }
            else if(strcmp(attrib, "heat_res_dmg") == 0)        { charge_module_list[i_ID].heat_res_dmg = value; }
            else if(strcmp(attrib, "energy_res_dmg") == 0)      { charge_module_list[i_ID].energy_res_dmg = value; }
            else if(strcmp(attrib, "dmg_type") == 0)            { charge_module_list[i_ID].dmg_type = value; }
            else { _ASSERT_MSG(false, "attempted to load invalid attribute for a charge module item"); }
        }
        else { //next item
            token = strtok(line_buffer, ":");
            strcpy(item_name, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            i_ID = atoi(token);
            LTRACE("reading item %s [ID = C-%d]...", item_name, i_ID);
            _ASSERT(i_ID < count);
            if(!first_item_read_started) { first_item_read_started = true; }
        }
        read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1); //read next line b4 next loop
    }
    return true;
}
u8 load_grapple_module_data(u32 count) {
    char line_buffer[MAX_LINE_SIZE+1];
    char item_name[MAX_LINE_SIZE];
    char* token;
    char attrib[MAX_LINE_SIZE];
    i32 value;
    u32 i_ID;
    u8 first_item_read_started = false;
    u32 index = 0;

    read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1);
    while(strcmp(line_buffer, "#end") != 0) {
        if(line_buffer[0] == '\0') {
            //empty line do nothing
        }
        else if(line_buffer[0] == '\t' || line_buffer[0] == ' ')
        {   //reading item attribute
            index = 0;
            while(line_buffer[index] == ' ' || line_buffer[index] == '\t')
                index++;
            _ASSERT(first_item_read_started == true);
            token = strtok(line_buffer+index, ":");
            strcpy(attrib, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            value = atoi(token);

            if(strcmp(attrib, "weight") == 0)                   { grapple_module_list[i_ID].weight = value; }
            else if(strcmp(attrib, "damage") == 0)              { grapple_module_list[i_ID].damage = value; }
            else if(strcmp(attrib, "heat_cost") == 0)           { grapple_module_list[i_ID].heat_cost = value; }
            else if(strcmp(attrib, "heat_damage") == 0)         { grapple_module_list[i_ID].heat_damage = value; }
            else if(strcmp(attrib, "max_heat_damage") == 0)     { grapple_module_list[i_ID].max_heat_damage = value; }
            else if(strcmp(attrib, "cooling_damage") == 0)      { grapple_module_list[i_ID].cooling_damage = value; }
            else if(strcmp(attrib, "energy_cost") == 0)         { grapple_module_list[i_ID].energy_cost = value; }
            else if(strcmp(attrib, "energy_damage") == 0)       { grapple_module_list[i_ID].energy_damage = value; }
            else if(strcmp(attrib, "max_energy_damage") == 0)   { grapple_module_list[i_ID].max_energy_damage = value; }
            else if(strcmp(attrib, "regen_damage") == 0)        { grapple_module_list[i_ID].regen_damage = value; }
            else if(strcmp(attrib, "phys_res_dmg") == 0)        { grapple_module_list[i_ID].phys_res_dmg = value; }
            else if(strcmp(attrib, "heat_res_dmg") == 0)        { grapple_module_list[i_ID].heat_res_dmg = value; }
            else if(strcmp(attrib, "energy_res_dmg") == 0)      { grapple_module_list[i_ID].energy_res_dmg = value; }
            else if(strcmp(attrib, "dmg_type") == 0)            { grapple_module_list[i_ID].dmg_type = value; }
            else { _ASSERT_MSG(false, "attempted to load invalid attribute for a grapple module item"); }
        }
        else { //next item
            token = strtok(line_buffer, ":");
            strcpy(item_name, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            i_ID = atoi(token);
            LTRACE("reading item %s [ID = G-%d]...", item_name, i_ID);
            _ASSERT(i_ID < count);
            if(!first_item_read_started) { first_item_read_started = true; }
        }
        read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1); //read next line b4 next loop
    }
    return true;
}

u8 load_drone_data(u32 count) {
    char line_buffer[MAX_LINE_SIZE+1];
    char item_name[MAX_LINE_SIZE];
    char* token;
    char attrib[MAX_LINE_SIZE];
    i32 value;
    u32 i_ID;
    u8 first_item_read_started = false;
    u32 index = 0;

    read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1);
    while(strcmp(line_buffer, "#end") != 0) {
        if(line_buffer[0] == '\0') {
            //empty line do nothing
        }
        else if(line_buffer[0] == '\t' || line_buffer[0] == ' ')
        {   //reading item attribute
            index = 0;
            while(line_buffer[index] == ' ' || line_buffer[index] == '\t')
                index++;
            _ASSERT(first_item_read_started == true);
            token = strtok(line_buffer+index, ":");
            strcpy(attrib, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            value = atoi(token);

            if(strcmp(attrib, "weight") == 0)                   { drone_list[i_ID].weight = value; }
            else if(strcmp(attrib, "damage") == 0)              { drone_list[i_ID].damage = value; }
            else if(strcmp(attrib, "heat_cost") == 0)           { drone_list[i_ID].heat_cost = value; }
            else if(strcmp(attrib, "heat_damage") == 0)         { drone_list[i_ID].heat_damage = value; }
            else if(strcmp(attrib, "max_heat_damage") == 0)     { drone_list[i_ID].max_heat_damage = value; }
            else if(strcmp(attrib, "cooling_damage") == 0)      { drone_list[i_ID].cooling_damage = value; }
            else if(strcmp(attrib, "energy_cost") == 0)         { drone_list[i_ID].energy_cost = value; }
            else if(strcmp(attrib, "energy_damage") == 0)       { drone_list[i_ID].energy_damage = value; }
            else if(strcmp(attrib, "max_energy_damage") == 0)   { drone_list[i_ID].max_energy_damage = value; }
            else if(strcmp(attrib, "regen_damage") == 0)        { drone_list[i_ID].regen_damage = value; }
            else if(strcmp(attrib, "phys_res_dmg") == 0)        { drone_list[i_ID].phys_res_dmg = value; }
            else if(strcmp(attrib, "heat_res_dmg") == 0)        { drone_list[i_ID].heat_res_dmg = value; }
            else if(strcmp(attrib, "energy_res_dmg") == 0)      { drone_list[i_ID].energy_res_dmg = value; }
            else if(strcmp(attrib, "dmg_type") == 0)            { drone_list[i_ID].dmg_type = value; }
            else { _ASSERT_MSG(false, "attempted to load invalid attribute for a drone item"); }
        }
        else { //next item
            token = strtok(line_buffer, ":");
            strcpy(item_name, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            i_ID = atoi(token);
            LTRACE("reading item %s [ID = D-%d]...", item_name, i_ID);
            _ASSERT(i_ID < count);
            if(!first_item_read_started) { first_item_read_started = true; }
        }
        read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1); //read next line b4 next loop
    }
    return true;
}

u8 load_module_data(u32 count) {
    char line_buffer[MAX_LINE_SIZE+1];
    char item_name[MAX_LINE_SIZE];
    char* token;
    char attrib[MAX_LINE_SIZE];
    i32 value;
    u32 i_ID;
    u8 first_item_read_started = false;
    u32 index = 0;

    read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1);
    while(strcmp(line_buffer, "#end") != 0) {
        if(line_buffer[0] == '\0') {
            //empty line do nothing
        }
        else if(line_buffer[0] == '\t' || line_buffer[0] == ' ')
        {   //reading item attribute
            index = 0;
            while(line_buffer[index] == ' ' || line_buffer[index] == '\t')
                index++;
            _ASSERT(first_item_read_started == true);
            token = strtok(line_buffer+index, ":");
            strcpy(attrib, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            value = atoi(token);

            if(strcmp(attrib, "weight") == 0)                   { module_list[i_ID].weight = value; }
            else if(strcmp(attrib, "hp") == 0)                  { module_list[i_ID].hp = value; }
            else if(strcmp(attrib, "heat") == 0)                { module_list[i_ID].heat = value; }
            else if(strcmp(attrib, "energy") == 0)              { module_list[i_ID].energy = value; }
            else if(strcmp(attrib, "cooling") == 0)             { module_list[i_ID].cooling = value; }
            else if(strcmp(attrib, "energy_regen") == 0)        { module_list[i_ID].energy_regen = value; }
            else if(strcmp(attrib, "phys_res") == 0)            { module_list[i_ID].phys_res = value; }
            else if(strcmp(attrib, "heat_res") == 0)            { module_list[i_ID].heat_res = value; }
            else if(strcmp(attrib, "energy_res") == 0)          { module_list[i_ID].energy_res = value; }
            else { _ASSERT_MSG(false, "attempted to load invalid attribute for a module item"); }
        }
        else { //next item
            token = strtok(line_buffer, ":");
            strcpy(item_name, token);
            token = strtok(NULL, ":");
            _ASSERT(is_number(token) == true);
            i_ID = atoi(token);
            LTRACE("reading item %s [ID = M-%d]...", item_name, i_ID);
            _ASSERT(i_ID < count);
            if(!first_item_read_started) { first_item_read_started = true; }
        }
        read_line(item_data_file, line_buffer, MAX_LINE_SIZE+1); //read next line b4 next loop
    }
    return true;
}

void free_item_data() {
    _ASSERT(torso_list              != 0);
    _ASSERT(leg_list                != 0);
    _ASSERT(side_weapon_list        != 0);
    _ASSERT(top_weapon_list         != 0);
    _ASSERT(teleport_module_list    != 0);
    _ASSERT(charge_module_list      != 0);
    _ASSERT(grapple_module_list     != 0);
    _ASSERT(drone_list              != 0);
    _ASSERT(module_list             != 0);

    free(torso_list);
    free(leg_list);
    free(side_weapon_list);
    free(top_weapon_list);
    free(teleport_module_list);
    free(charge_module_list);
    free(grapple_module_list);
    free(drone_list);
    free(module_list);

    torso_list = 0;
    leg_list  = 0;
    side_weapon_list = 0;
    top_weapon_list = 0;
    teleport_module_list = 0;
    grapple_module_list = 0;
    drone_list = 0;
    module_list = 0;

    LINFO("item data uninitialized");
}

item_torso get_torso(i32 i_ID) {
    _ASSERT(i_ID < item_count_list[I_TORSO]);
    item_torso temp;
    temp.weight         = torso_list[i_ID].weight;
    temp.hp             = torso_list[i_ID].hp;
    temp.heat           = torso_list[i_ID].heat;
    temp.energy         = torso_list[i_ID].energy;
    temp.phys_res       = torso_list[i_ID].phys_res;
    temp.heat_res       = torso_list[i_ID].heat_res;
    temp.energy_res     = torso_list[i_ID].energy_res;
    temp.cooling        = torso_list[i_ID].cooling;
    temp.energy_regen   = torso_list[i_ID].energy_regen;
    return temp;
}

item_leg get_leg(i32 i_ID) {
    _ASSERT(i_ID < item_count_list[I_LEG]);
    item_leg temp;
    temp.weight             = leg_list[i_ID].weight;
    temp.hp                 = leg_list[i_ID].hp;
    temp.uses               = leg_list[i_ID].uses;
    temp.damage             = leg_list[i_ID].damage;
    temp.heat_cost          = leg_list[i_ID].heat_cost;
    temp.heat_damage        = leg_list[i_ID].heat_damage;
    temp.max_heat_damage    = leg_list[i_ID].max_heat_damage;
    temp.cooling_damage     = leg_list[i_ID].cooling_damage;
    temp.energy_cost        = leg_list[i_ID].energy_cost;
    temp.energy_damage      = leg_list[i_ID].energy_damage;
    temp.max_energy_damage  = leg_list[i_ID].max_energy_damage;
    temp.regen_damage       = leg_list[i_ID].regen_damage;
    temp.phys_res_dmg       = leg_list[i_ID].phys_res_dmg;
    temp.heat_res_dmg       = leg_list[i_ID].heat_res_dmg;
    temp.energy_res_dmg     = leg_list[i_ID].energy_res_dmg;
    temp.displace           = leg_list[i_ID].displace;
    temp.dmg_type           = leg_list[i_ID].dmg_type;
    return temp;
}

item_side_weapon get_side_weapon(i32 i_ID) {
    _ASSERT(i_ID < item_count_list[I_SIDE_WEAPON]);
    item_side_weapon temp;
    temp.weight             = side_weapon_list[i_ID].weight;
    temp.uses               = side_weapon_list[i_ID].uses;
    temp.min_range          = side_weapon_list[i_ID].min_range;
    temp.max_range          = side_weapon_list[i_ID].max_range;
    temp.damage             = side_weapon_list[i_ID].damage;
    temp.heat_cost          = side_weapon_list[i_ID].heat_cost;
    temp.heat_damage        = side_weapon_list[i_ID].heat_damage;
    temp.max_heat_damage    = side_weapon_list[i_ID].max_heat_damage;
    temp.cooling_damage     = side_weapon_list[i_ID].cooling_damage;
    temp.energy_cost        = side_weapon_list[i_ID].energy_cost;
    temp.energy_damage      = side_weapon_list[i_ID].energy_damage;
    temp.max_energy_damage  = side_weapon_list[i_ID].max_energy_damage;
    temp.regen_damage       = side_weapon_list[i_ID].regen_damage;
    temp.phys_res_dmg       = side_weapon_list[i_ID].phys_res_dmg;
    temp.heat_res_dmg       = side_weapon_list[i_ID].heat_res_dmg;
    temp.energy_res_dmg     = side_weapon_list[i_ID].energy_res_dmg;
    temp.displace           = side_weapon_list[i_ID].displace;
    temp.dmg_type           = side_weapon_list[i_ID].dmg_type;
    return temp;
}

item_top_weapon get_top_weapon(i32 i_ID) {
    _ASSERT(i_ID < item_count_list[I_TOP_WEAPON]);
    item_top_weapon temp;
    temp.weight             = top_weapon_list[i_ID].weight;
    temp.uses               = top_weapon_list[i_ID].uses;
    temp.min_range          = top_weapon_list[i_ID].min_range;
    temp.max_range          = top_weapon_list[i_ID].max_range;
    temp.damage             = top_weapon_list[i_ID].damage;
    temp.heat_cost          = top_weapon_list[i_ID].heat_cost;
    temp.heat_damage        = top_weapon_list[i_ID].heat_damage;
    temp.max_heat_damage    = top_weapon_list[i_ID].max_heat_damage;
    temp.cooling_damage     = top_weapon_list[i_ID].cooling_damage;
    temp.energy_cost        = top_weapon_list[i_ID].energy_cost;
    temp.energy_damage      = top_weapon_list[i_ID].energy_damage;
    temp.max_energy_damage  = top_weapon_list[i_ID].max_energy_damage;
    temp.regen_damage       = top_weapon_list[i_ID].regen_damage;
    temp.phys_res_dmg       = top_weapon_list[i_ID].phys_res_dmg;
    temp.heat_res_dmg       = top_weapon_list[i_ID].heat_res_dmg;
    temp.energy_res_dmg     = top_weapon_list[i_ID].energy_res_dmg;
    temp.displace           = top_weapon_list[i_ID].displace;
    temp.dmg_type           = top_weapon_list[i_ID].dmg_type;
    return temp;
}

item_teleport_module get_teleport_module(i32 i_ID) {
    _ASSERT(i_ID < item_count_list[I_TELEPORT_MODULE]);
    item_teleport_module temp;
    temp.weight             = teleport_module_list[i_ID].weight;
    temp.uses               = teleport_module_list[i_ID].uses;
    temp.damage             = teleport_module_list[i_ID].damage;
    temp.heat_cost          = teleport_module_list[i_ID].heat_cost;
    temp.heat_damage        = teleport_module_list[i_ID].heat_damage;
    temp.max_heat_damage    = teleport_module_list[i_ID].max_heat_damage;
    temp.cooling_damage     = teleport_module_list[i_ID].cooling_damage;
    temp.energy_cost        = teleport_module_list[i_ID].energy_cost;
    temp.energy_damage      = teleport_module_list[i_ID].energy_damage;
    temp.max_energy_damage  = teleport_module_list[i_ID].max_energy_damage;
    temp.regen_damage       = teleport_module_list[i_ID].regen_damage;
    temp.phys_res_dmg       = teleport_module_list[i_ID].phys_res_dmg;
    temp.heat_res_dmg       = teleport_module_list[i_ID].heat_res_dmg;
    temp.energy_res_dmg     = teleport_module_list[i_ID].energy_res_dmg;
    temp.dmg_type           = teleport_module_list[i_ID].dmg_type;
    return temp;
}

item_charge_module get_charge_module(i32 i_ID) {
    _ASSERT(i_ID < item_count_list[I_CHARGE_MODULE]);
    item_charge_module temp;
    temp.weight             = charge_module_list[i_ID].weight;
    temp.uses               = charge_module_list[i_ID].uses;
    temp.damage             = charge_module_list[i_ID].damage;
    temp.heat_cost          = charge_module_list[i_ID].heat_cost;
    temp.heat_damage        = charge_module_list[i_ID].heat_damage;
    temp.max_heat_damage    = charge_module_list[i_ID].max_heat_damage;
    temp.cooling_damage     = charge_module_list[i_ID].cooling_damage;
    temp.energy_cost        = charge_module_list[i_ID].energy_cost;
    temp.energy_damage      = charge_module_list[i_ID].energy_damage;
    temp.max_energy_damage  = charge_module_list[i_ID].max_energy_damage;
    temp.regen_damage       = charge_module_list[i_ID].regen_damage;
    temp.phys_res_dmg       = charge_module_list[i_ID].phys_res_dmg;
    temp.heat_res_dmg       = charge_module_list[i_ID].heat_res_dmg;
    temp.energy_res_dmg     = charge_module_list[i_ID].energy_res_dmg;
    temp.dmg_type           = charge_module_list[i_ID].dmg_type;
    return temp;
}

item_grapple_module get_grapple_module(i32 i_ID) {
    _ASSERT(i_ID < item_count_list[I_GRAPPLE_MODULE]);
    item_grapple_module temp;
    temp.weight             = grapple_module_list[i_ID].weight;
    temp.uses               = grapple_module_list[i_ID].uses;
    temp.damage             = grapple_module_list[i_ID].damage;
    temp.heat_cost          = grapple_module_list[i_ID].heat_cost;
    temp.heat_damage        = grapple_module_list[i_ID].heat_damage;
    temp.max_heat_damage    = grapple_module_list[i_ID].max_heat_damage;
    temp.cooling_damage     = grapple_module_list[i_ID].cooling_damage;
    temp.energy_cost        = grapple_module_list[i_ID].energy_cost;
    temp.energy_damage      = grapple_module_list[i_ID].energy_damage;
    temp.max_energy_damage  = grapple_module_list[i_ID].max_energy_damage;
    temp.regen_damage       = grapple_module_list[i_ID].regen_damage;
    temp.phys_res_dmg       = grapple_module_list[i_ID].phys_res_dmg;
    temp.heat_res_dmg       = grapple_module_list[i_ID].heat_res_dmg;
    temp.energy_res_dmg     = grapple_module_list[i_ID].energy_res_dmg;
    temp.dmg_type           = grapple_module_list[i_ID].dmg_type;
    return temp;
}

item_drone get_drone(i32 i_ID) {
    _ASSERT(i_ID < item_count_list[I_DRONE]);
    item_drone temp;
    temp.weight             = drone_list[i_ID].weight;
    temp.damage             = drone_list[i_ID].damage;
    temp.heat_cost          = drone_list[i_ID].heat_cost;
    temp.heat_damage        = drone_list[i_ID].heat_damage;
    temp.max_heat_damage    = drone_list[i_ID].max_heat_damage;
    temp.cooling_damage     = drone_list[i_ID].cooling_damage;
    temp.energy_cost        = drone_list[i_ID].energy_cost;
    temp.energy_damage      = drone_list[i_ID].energy_damage;
    temp.max_energy_damage  = drone_list[i_ID].max_energy_damage;
    temp.regen_damage       = drone_list[i_ID].regen_damage;
    temp.phys_res_dmg       = drone_list[i_ID].phys_res_dmg;
    temp.heat_res_dmg       = drone_list[i_ID].heat_res_dmg;
    temp.energy_res_dmg     = drone_list[i_ID].energy_res_dmg;
    temp.dmg_type           = drone_list[i_ID].dmg_type;
    return temp;
}

item_module get_module(i32 i_ID) {
    _ASSERT(i_ID < item_count_list[I_MODULE]);
    item_module temp;
    temp.weight             = module_list[i_ID].weight;
    temp.hp                 = module_list[i_ID].hp;
    temp.heat               = module_list[i_ID].heat;
    temp.energy             = module_list[i_ID].energy;
    temp.cooling            = module_list[i_ID].cooling;
    temp.energy_regen       = module_list[i_ID].energy_regen;
    temp.phys_res           = module_list[i_ID].phys_res;
    temp.heat_res           = module_list[i_ID].heat_res;
    temp.energy_res         = module_list[i_ID].energy_res;
    return temp;
}
