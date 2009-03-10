/*    /cmds/creator/goto.c
 *    from the Dead Souls LPC Library
 *    a command to take a cre anywhere in the game
 *    created by Descartes of Borg 930906
 *    modified for NM IV 950331
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    object ob, dude;
    int err;

    if(!str) return "Goto where?";
    ob = find_player(lower_case(str));
    //if(!ob) ob = find_living(lower_case(str));
    if(ob) dude = ob;
    if(ob && objectp(ob) && (!ob->GetInvis() || !archp(ob)) && 
            ob = room_environment(ob)) {
        if(ob == room_environment(this_player())) {
            message("my_action", "You twitch.", this_player());
            if(hiddenp(this_player())) return 1;
            message("other_action", (string)this_player()->GetName()+
                    " twitches.", ob, ({ this_player() }));
            return 1;
        }
    }
    if(ob && ob->GetInvis() && creatorp(ob) && !archp(this_player())) ob = 0;
    if(!ob) str = absolute_path((string)this_player()->query_cwd(), str);
    if(ob) {
        //debug("ob: "+identify(ob),"red");
        this_player()->eventMoveLiving(ob);
        return 1;
    }

    if(last_string_element(path_prefix(path_prefix(str)),"/") != "virtual"){
        int x, y, z;
        string file = last_string_element(str,"/");
        if(sscanf(file,"%d,%d,%d", x, y, z) != 3)
            sscanf(file,"%d,%d", x, y);
        if(!undefinedp(x) && !undefinedp(y)){
            mixed rmap = ROOMS_D->GetGrid(file);
            if(rmap && rmap["room"]){
                if(!file_exists(str) && !file_exists(str + ".c")){
                    str = rmap["room"];
                }
            }
        }
        else if(!file_exists(str) && !file_exists(str + ".c")){
            write("Location not found.");
            return 1;
        }
    }

    err = catch( ob = load_object(str) );

    if(err || !ob) {
        write("\n\nCould not load that location.");
        return 1;
    }
    else this_player()->eventMoveLiving(ob);
    return 1;
}

void help() {
    message("help",
            "Syntax: <goto [living thing|file]>\n\n"
            "This command will move you to where the living thing is if it can "
            "be found, otherwise it will search for the file named and try to "
            "move you into that file.\n\nSee also: home, move, trans, expel.",
            this_player()
           );
}
