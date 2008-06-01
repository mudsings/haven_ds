#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    write("Screen: \t\t"+identify(this_player()->GetScreen()));
    write("Terminal: \t\t"+this_player()->GetTerminal());
    write("Brief mode: \t\t"+ ( (this_player()->GetBriefMode()) ? "on" : "off" ));
    write("Channel message colors: "+ ( (this_player()->GetNoChanColors()) ? "off" : "on" ));
    write("Playerkiller mode: \t"+ ( (this_player()->GetPK()) ? "on" : "off" ));
    write("Mute mode: \t\t"+ ( (this_player()->GetProperty("mute")) ? "on" : "off" ));
    write("Gag mode: \t\t"+ ( (this_player()->GetProperty("gag")) ? "on" : "off" ));
    write("Wimpy mode: \t\t"+ ( ((int)this_player()->GetWimpy()) ? "on" : "off" ));
    write("Automap mode: \t\t"+ ( this_player()->GetProperty("automapping") ? "on" : "off" ));
    write("Annoyblock: \t\t"+ ( (this_player()->GetAnnoyblock()) ? "on" : "off" ));
    if(creatorp(this_player())){ 
        write("Debug mode: \t\t"+ ( (this_player()->GetProperty("debug")) ? "on" : "off" ));
        write("Godmode: \t\t"+ ( (this_player()->GetGodMode()) ? "on" : "off" ));
        write("Wizvision: \t\t"+ ( (this_player()->GetWizVision()) ? "on" : "off" ));
    }
    return 1;
}

void help() {
    string de_bug;
    if(creatorp(this_player())) de_bug = ", debug, wizvision, godmode.";
    else de_bug = ".";
    message("help", "Syntax: <env>\n\n"
      "Displays some basic interface and play settings. " +
      "See also: brief, terminal, screen, chancolors, pk, mute, gag, wimpy, automap, annoyblock"+de_bug, this_player());
}
