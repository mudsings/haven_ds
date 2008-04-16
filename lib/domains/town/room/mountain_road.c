#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Fort Road");
    SetNightLong("You are on a road at the northern edge of the village. To the south is the heart of the town. North, the road leads into the darkness.");
    SetDayLong("You are on a road running north and south. To the south is the heart of the town. To the north the road runs along a narrow ridge, leading up to what looks like a fortress.");
    SetItems( ([
        ({"rubble","stone","stones","road"}) : "Looks like "
        "the road is still being built. Further travel north "
        "is impossible.",
        ({"mountains","beautiful","snow-capped mountains"}) : "Beautiful "
        "mountains, far off in the northern distance."
      ]) );
    SetExits( ([
        "south" : "/domains/town/room/road",
      ]) );
    AddTerrainType(T_ROAD);
    SetFlyRoom("/domains/town/virtual/sky/28,2,1");
    AddItem(new("/domains/town/obj/lamp"));
    if(file_exists("/domains/fort/room/f_road4.c")){
        AddExit("north", "/domains/fort/room/f_road4.c");
    }
}
void init(){
    ::init();
}
