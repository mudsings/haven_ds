#include <lib.h>
#include <rooms.h>

inherit LIB_DAEMON;

int cmd(string str) {
    object *rooms = ({});

    write("Compiling list of loaded rooms...");
    flush_messages(this_player());

    rooms = filter(objects(), (: inherits(LIB_ROOM, $1) :) );


    if(archp(previous_object())){

        foreach(object room in rooms){
            write("Updating: "+base_name(room));
            update(base_name(room));
        }

        update(ROOM_FURNACE);
        reap_other();
        reap_other();

        write("Done.");

        return 1;
    }
    write("You are not admin. This is command forbidden.");
    return 1;
}


void help() {
    write("Syntax: <resetall>\n\n"+
      "Unloads and reloads all rooms currently in memory.\n"+
      "This tends to destroy all non-interactive objects in "+
      "the rooms, and dumps all players at the start room "+
      "or The Void, so use this command with caution.\n");
}
