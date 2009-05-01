#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string *lines;
    string buff, file, clone, base;
    int scr, ret;
    object ob;
    //tc("args: "+args);
    if( !args ) return "You must specify a file to destruct.";
    sscanf(args, "%s#%s", base, clone);
    if(clone){
        ob = find_object(args);
        if(!ob){
            write(args + " was not found.");
            return 1;
        }
        ret = ob->eventDestruct();
        if(!ret || ob) destruct(ob);
        if(ob){
            write("Failed to destruct "+args);
            return 1;
        }
        write(args + " destructed.");
        return 1;
    }
        
    file = absolute_path((string)this_player()->query_cwd(), args);
    //file = lpc_file(file);
    //tc("1 file: "+file);
    if(!(ob = find_object(file)) && !grepp(args, "/")){
        file = DEFINES_D->GetDefine(args);
        //file = lpc_file(file);
    }
    //tc("2 file: "+file);
    if(!(ob = find_object(file))) return "Object " + args + " not found.";
    ret = ob->eventDestruct();
    if(!ret || ob) destruct(ob);
    if(ob){
        write(args + " could not be destructed.");
    }
    else {
        write(args + " destructed.");
    }
    return 1;
}

void help() {
    message("help", "Syntax: destfile <file>\n\n"
            "Destructs the instance of the loaded object of that name."
            "\nExamples:\ndestfile /realms/cratylus/area/obj/table#527\n"
            "destfile /daemon/skills\n"
            "destfile LIB_CONNECT"
            "\n\n"
            "See also: dest, zap, clean, update, reload", this_player());
}
