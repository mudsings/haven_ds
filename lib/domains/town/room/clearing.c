#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(40);
    SetNightLight(10);
    SetDayLight(30);
    SetShort("Forest Clearing");
    SetLong("This is a small clearing in a path through the forest. The path leads west into a small valley, and east toward a town.");
    SetExits( ([ 
        "west" : "/domains/town/room/valley",
        "east" : "/domains/town/room/forest_path1",
      ]) );
    SetItems( ([
        "clearing" : "An area with few trees.",
        "path" : "This forest path runs east "
        "and west. A small trail goes north of here.",
        ({"forest","wood","woods"}) : "This is a "
        "dark, creepy forest. You are standing on a "
        "path in a clearing, so it's a bit brighter here.",
        ({ "small trail","trail"}) : "A small trail leading into the depths "
        "of the forest to the north.",
        "valley" : "A valley is west of here.",
        "town" : "A town, far in the eastern distance."
      ]) );
    SetFlyRoom("/domains/town/virtual/sky/23,100000,1");
}
void init(){
    ::init();
}
