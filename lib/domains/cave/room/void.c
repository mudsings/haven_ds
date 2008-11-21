#include <lib.h>
#include <rooms.h>

inherit "/domains/cave/etc/cave_room";

void create() {
    ::create();
    SetAmbientLight(30);
    SetShort("the void");
    SetLong("The void.  Go down to get out.");
    SetExits( ([ "down" : ROOM_START ]) );
    SetNoModify(1);
}
void init(){
    ::init();
}
