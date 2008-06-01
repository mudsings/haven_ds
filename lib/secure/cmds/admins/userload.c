#include <lib.h>
#include <dirs.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string arg){
    int ret;
    if(!archp(previous_object())) return 0;
    if(!arg || arg == "me") arg = this_player()->GetKeyName();
    if(!find_player(arg)){
        write("The user was not found.");
        return 1;
    }
    ret = RELOAD_D->ReloadPlayer(arg, 1);
    if(!ret) write("An error occurred.");
    else write("Done.");

    return 1;
}

int help()
{
    write( @EndText
Syntax: userload <username>
Reloads the code for users, creates a new body for the specified
user, and switches them into that new body. This command is useful
for testing new functionality in the user object without having
to quit.
See also: reload, update, warmboot
EndText
    );
    return 1;
}
