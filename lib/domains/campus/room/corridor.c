#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Corridor");
    SetLong("You are in an east-west corridor on the first "
      "floor of the Virtual Campus administrative building. "
      "There is a door south of here, leading into the "
      "Admissions office. The building's snack bar lies to the north.");
    SetItems( (["corridor" : "This is a carpeted corridor, leading west.",
        "floor" : "The carpet is purple, and somewhat institutional.",
        "carpet" : "The carpet is purple, and somewhat institutional."]) );
    SetExits( ([
        "south" : "/domains/campus/room/start",
        "west" : "/domains/campus/room/corridor3",
        "north" : "/domains/campus/room/snack",
        "east" : "/domains/campus/room/corridor2.c",
      ]) );
    SetProperty("no attack", 1);
    SetDoor("south", "/domains/campus/doors/plain_door");
}
int CanReceive(object ob) {
    if(ob && ob->GetRace() == "rodent"){
        message("info","You are repelled by rodenticide.",ob);
        return 0;
    }
    return 1;
}
void init(){
    ::init();
}
