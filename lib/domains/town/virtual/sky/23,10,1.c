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
    SetLong("You are in the air above a vast sea that stretches from horizon to horizon.");
    SetClimate("temperate");
    SetTown("wilderness");
    SetTerrainType(T_MIDAIR);
    SetMedium(MEDIUM_AIR);
    SetItems( ([
        ({ "sea", "ocean" }) : "A seemingly endless body of water.",
      ]) );
    SetExits( ([
        "north" : "/domains/town/virtual/sky/23,11,1",
        "south" : "/domains/town/virtual/sky/23,9,1",
        "east" : "/domains/town/virtual/sky/24,10,1",
        "southeast" : "/domains/town/virtual/sky/24,9,1",
        "northeast" : "/domains/town/virtual/sky/24,11,1",
        "west" : "/domains/town/virtual/sky/22,10,1",
        "southwest" : "/domains/town/virtual/sky/22,9,1",
        "northwest" : "/domains/town/virtual/sky/22,11,1",
        "up" : "/domains/town/virtual/sky/23,10,2",
        "down" : "/domains/fort/room/bpath2.c",
      ]) );
}
void init(){
    ::init();
}
