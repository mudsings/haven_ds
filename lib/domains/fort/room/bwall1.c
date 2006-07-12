/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 15:24:23 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(25);
    SetShort("bailey wall");
    SetDayLong("bailey wall, day");
    SetNightLong("bailey wall, night");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([          ({"wall", "walls"}) : "These walls appear extremely strong and thick, as one would expect of a military fortification." ,
	({"battlement", "battlements"}) : "These are defensive structures along the top of the wall that provide cover for defenders."]));
    SetInventory(([]));
    SetObviousExits("e, w");
    AddExit("east", MY_ROOM + "/" +"bwall2");
    AddExit("west", MY_ROOM + "/" +"west_wall_5");
    //funs
    //snuf
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras