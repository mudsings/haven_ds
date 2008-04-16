#include <medium.h>
#include <terrain_types.h>
#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetDayLight(30);
    SetNightLight(30);
    SetShort("The sky");
    SetLong("You are in the air above a fortress.");
    SetClimate("temperate");
    SetTown("wilderness");
    SetTerrainType(T_MIDAIR);
    SetMedium(MEDIUM_AIR);
    SetItems( ([
        ({ "sea", "ocean" }) : "A seemingly endless body of water.",
      ]) );
    SetExits( ([
        "north" : "/domains/town/virtual/sky/19,7,1",
        "south" : "/domains/town/virtual/sky/19,5,1",
        "east" : "/domains/town/virtual/sky/20,6,1",
        "southeast" : "/domains/town/virtual/sky/20,5,1",
        "northeast" : "/domains/town/virtual/sky/20,7,1",
        "west" : "/domains/town/virtual/sky/18,6,1",
        "southwest" : "/domains/town/virtual/sky/18,5,1",
        "northwest" : "/domains/town/virtual/sky/18,7,1",
        "up" : "/domains/town/virtual/sky/19,6,2",
        "down" : "/domains/fort/room/mstreet6.c",
      ]) );
}
void init(){
    ::init();
}
