#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The start room");
    SetLong("The default start room. To enter "+
      "a sample set of rooms, go down.");
    SetExits( ([ 
        "down" : "/domains/campus/room/start",
      ]) );
    SetNoModify(1);
}
void init(){
    ::init();
}
