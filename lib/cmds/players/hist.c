#include <lib.h>
#include <daemons.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if(!args) 
        this_player()->eventPrint("Syntax: <hist [channel]>");

    this_player()->eventPrint("Retrieving history...");

    if(args == "tell"){
        load_object("/secure/cmds/players/tell")->cmd("hist");
        return 1;
    }

    return CHAT_D->cmdLast(args);
    return 1;
}

string GetHelp(string topic) {
    return ("Syntax: <hist [channel]>\n"
      "Gives you a list of the hist of a channel ");
}
