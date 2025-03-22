#include "item_data.h"

#include "asserts.h"
#include "defines.h"

#include <stdlib.h>

item_torso*             torso_list = 0;
item_leg*               leg_list = 0;
item_side_weapon*       side_weapon_list = 0;
item_top_weapon*        top_weapon_list = 0;
item_teleport_module*   teleport_module_list = 0;
item_charge_module*     charge_module_list = 0;
item_grapple_module*    grapple_module_list = 0;
item_drone*             drone_list = 0;
item_module*            module_list = 0;

u8 load_item_data() {
    //TODO: open item file and get number of items for each item type
    //TODO: ^ load above info into properly formatted array and pass to init_item_data() to initialize item_data lists
    //TODO: go through file and populate item_data lists with info from file
    return true;
}

u8 init_item_data(u32* item_count_list, u32 list_size) {
    _ASSERT(list_size == I_TYPES_NUM);
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
    side_weapon_list        = (item_side_weapon*)malloc(sizeof(item_side_weapon) * (item_count_list[I_SIDE_WEAPON]));
    top_weapon_list         = (item_top_weapon*)malloc(sizeof(item_top_weapon) * (item_count_list[I_TOP_WEAPON]));
    teleport_module_list    = (item_teleport_module*)malloc(sizeof(item_teleport_module) * (item_count_list[I_TELEPORT_MODULE]));
    charge_module_list      = (item_charge_module*)malloc(sizeof(item_charge_module) * (item_count_list[I_CHARGE_MODULE]));
    grapple_module_list     = (item_grapple_module*)malloc(sizeof(item_grapple_module) * (item_count_list[I_GRAPPLE_MODULE]));
    drone_list              = (item_drone*)malloc(sizeof(item_drone) * (item_count_list[I_DRONE]));
    module_list             = (item_module*)malloc(sizeof(item_module) * (item_count_list[I_MODULE]));

    _ASSERT(torso_list              != 0);
    _ASSERT(leg_list                != 0);
    _ASSERT(side_weapon_list        != 0);
    _ASSERT(top_weapon_list         != 0);
    _ASSERT(teleport_module_list    != 0);
    _ASSERT(charge_module_list      != 0);
    _ASSERT(grapple_module_list     != 0);
    _ASSERT(drone_list              != 0);
    _ASSERT(module_list             != 0);

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
}
