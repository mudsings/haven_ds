#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    string ret;
    object dude;

    if(str && !creatorp(this_player()) ) {
	if(!find_player(str) || find_player(str) != this_player()){ 
	    write("You can only get quest information about yourself.");
	    return 1;
	}
    }
    if(str && !find_player(str)){
	write("That player isn't logged on.");
	return 1;
    }
    if(str) dude = find_player(str);
    else dude = this_player();
    if(!sizeof(dude->GetQuests())){
	if(dude == this_player()) write("You have completed no quests.");
	else write(dude->GetName()+" has completed no quests.");
	return 1;
    }

    if(dude == this_player()) ret = "You have completed the following quests:\n";
    else ret = dude->GetName()+" has completed the following quests:\n";

    ret += "----------------------------------------\n";

    foreach(string *quests in dude->GetQuests()){
	ret += quests[1] + "\n";
    }
    write(ret);
    return 1;
}

void help() {
    message("help",
      "Syntax: quests\n\n"
      "This command lists the quests you have completed.\n\n",
      this_player() );
}

