#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Saquivor Road");
    SetNightLong("Illuminated by lamplight here is Saquivor road, leading north-south through the village. To the east is the village pub, the legendary watering hole used by thousands of rogues, paladins, and everyone in between. The Healers' Guild is west. To the south is a major intersection. The road continues north into the gloom.");
    SetDayLong("You are on Saquivor road, leading north-south through the village. To the east is the village pub, the legendary watering hole used by thousands of rogues, paladins, and everyone in between. The Healers' Guild is west, and to the north you can see a road leading up to the plateau where the Fortress on the Frontiers stands. To the south is a major intersection.");
    SetItems( ([
        ({"pub","tavern","village pub"}) : "The is the town's "
        "legendary watering hole, where refreshments are served "
        "and tall tales are swapped.",
        ({"guild","healers guild"}) : "In this building is "
        "the office of the only doctor in town.",
        "clouds" : "Little puffy clouds surround the "
        "peaks of the northern mountains.",
        ({"northern mountains","mountains","range","mountain range"}): 
        "Majestic mountains are far to the north, seeming "
        "almost purple from here.",
        "intersection" : "The main town crossroads is south."
      ]) );
    AddTerrainType(T_ROAD);
    SetExits( ([ 
        "east" : "/domains/town/room/tavern",
        "south" : "/domains/town/room/vill_road1",
        "west" : "/domains/town/room/healer",
        "north" : "/domains/town/room/mountain_road",
      ]) );
    SetInventory(([
        "/domains/town/weap/knife" :1,
        "/domains/town/npc/beggar" :1
      ]) );

    SetEnters( ([
        "pub" : "/domains/town/room/tavern",
        "guild" : "/domains/town/room/healer",
        "healers guild" : "/domains/town/room/healer"
      ]) );
    SetNoModify(0);
    SetFlyRoom("/domains/town/virtual/sky/28,1,1");
    AddItem(new("/domains/town/obj/lamp"));
}

void init(){
    ::init();
}
