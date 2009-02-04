#include <lib.h>
#ifndef CED_DISABLED 
#define CED_DISABLED 1
#endif

inherit LIB_DAEMON;

mixed cmd(string args) {
    int x;

    if( args == "" || !args ) 
        args = user_path((string)this_player()->GetKeyName()) + "tmp.edit";
    if( (int)this_player()->GetForced() ) {
        message("system", "Someone tried to force you to edit "+ args +"\n" +
                identify(previous_object(-1)), this_player());
        return 0;
    }

    if(!this_player()->GetProperty("cedWarned")){
        write("This is the first time you've used ced. If you get stuck, "
                "simply hit: Ctrl-Q");
        this_player()->SetProperty("cedWarned", 1);
    }
    args = absolute_path( (string)this_player()->query_cwd(), args );
    if( (x = file_size(args)) == -2 ) 
        return "You cannot edit a directory!";
    else if( x == -1 )
        message("editor", args + ", new file, starting in input mode.\n",
                this_player());
    else message("editor", args + ", " + x + " bytes\n", this_player());
    //this_player()->eventEdit(args, 1);
    this_player()->SetCediting(1, args);
    return 1;
}

void help() {
    string ret = "";
    ret += "Syntax: sed FILE\n\n";
    ret += "This command starts a screen-mode editing session on the "+
        "specified file. Note that without charmode enabled, this "+
        "will not work. Special commands are issued by pressing the "+
        "\"Control\" key and another key at the same time. For "+
        "example, to quit without saving, you press and hold "+
        "the control key, then the 'q' key. This is known as Ctrl-Q. "+
        "The special commands for sed are:\n\n"+
        "Ctrl-Q -- quit without saving\n"+
#if CED_DISABLED
        "Ctrl-X -- %^RED%^DISABLED FUNCTION%^RESET%^\n"+
        "Ctrl-S -- %^RED%^DISABLED FUNCTION%^RESET%^\n"+
#else
        "Ctrl-X -- quit and save\n"+
        "Ctrl-S -- save\n"+
#endif
        "Ctrl-I -- enable insert mode\n"+
        "Ctrl-O -- enable overstrike mode\n"+
        "Ctrl-L -- refresh the page\n"+
        "Ctrl-F -- scroll forward one page\n"+
        "Ctrl-B -- scroll backward one page\n"+
        "Ctrl-D -- delete current line\n"+
        "Ctrl-G -- Specify a line to go to\n"+
        "\nTo navigate in the screen, use the arrow keys on your keyboard. "+
        "Note that sed is extremely basic and experimental. Do not "+
        "expect things like search and replace to work."+
        "\nSee also: ed, charmode"+
#if CED_DISABLED
        "\n\n Please note that due to its alpha status, saving files in "+
        "ced is disabled for now."+
#endif
        "";
    message("help", ret, this_player());
}
