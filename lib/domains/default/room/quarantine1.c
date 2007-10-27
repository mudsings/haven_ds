#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Disease Lab Prep Room");
    SetLong("This room is designed to repel the spread of diseases that are tested in the room north of here. It is also where you can get a disease repellent collar, so you may work in the quarantine area without becoming infected. To infect the test subjects use the medical tricorder. To make all of them healthy again, type 'update', which will reset the room. The test subjects are waiting for you north of here.");
    SetInventory(([
        "/domains/default/obj/collarchest" : 1,
      ]));
    SetExits( ([
        "south" : "/domains/default/room/wiz_corr_east",
        "north" : "/domains/default/room/quarantine2.c",
      ]) );

    SetProperty("no attack", 1);

}
int CanReceive(object ob) {
    object *inv;
    string taxonomy;

    if(inherits(LIB_GERM,ob)) return 0;

    if(living(ob) && !creatorp(ob) && !present("testchar badge",ob)) {
        message("info","Creator staff only, sorry.", ob);
        return 0;
    }

    inv = deep_inventory(ob);
    foreach(object thing in inv){
        if(inherits(LIB_GERM,thing)) {
            write("%^YELLOW%^A parasite has been discovered on your body! It is being automatically removed.%^RESET%^");
            if(taxonomy = thing->GetGermName()) write("%^YELLOW%^Removing: "+taxonomy+".%^RESET%^");
            thing->eventMove(ROOM_FURNACE);
        }
    }

    return room::CanReceive();
}

void init(){
    ::init();
}
