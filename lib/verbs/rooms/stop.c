/*    /verbs/rooms/stop.c
 *    from the Dead Souls Object Library
 *    stop STR
 *    created by Descartes of Borg 951016
 */

#include <lib.h>
#include "include/stop.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("stop");
    SetRules("STR");
}

mixed can_stop_str(string str) { 
    return this_player()->CanStop(this_player(), str);
}

mixed do_stop_str(string str) {
    return this_player()->eventStop(this_player(), str);
}

string GetHelp(string str) {
    return ("Syntax: stop [ fishing | fighting ]\n\n"
      "Puts an end to the specified activity.\n\n"
      "See also: cast, fish, attack");
}
