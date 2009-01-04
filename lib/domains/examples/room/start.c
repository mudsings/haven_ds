#include <lib.h>
#include <rooms.h>
#include "ex.h"

inherit LIB_ROOM;


void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("a blank room");
    SetLong("A featureless area. The example rooms are north.");

    SetExits( ([
                "north" : EXPATH + "entrance"
                ]) );
}
void init(){
    ::init();
}
