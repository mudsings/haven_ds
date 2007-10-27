#include <lib.h>
#include <objects.h>

string *ExemptArray = ({ LIB_SHADOW, OBJ_SNOOPER, LIB_DOOR, LIB_ROOM, LIB_SERVER, LIB_FTP_DATA_CONN});

void  reap_dummies(){
    //
    // destroys any dummy objects that do not
    // have an environment
    //
    object *objects;
    object *dummies;

    objects = objects();
    dummies = ({});

    foreach(object ob in objects){
        if(base_name(ob) == "/lib/std/dummy"){
            dummies += ({ ob });
        }
    }

    foreach(object dummy in dummies){
        if(!environment(dummy)) dummy->eventDestruct();
    }
}

varargs void reap_other(string s1){
    //
    // destroys any cloned objects that
    // do not have an environment
    //
    string s2;
    object *objects;
    object *others;

    objects = objects();
    others = ({});

    foreach(object ob in objects){
        if(sscanf(file_name(ob),"%s#%s",s1,s2) > 0) {
            if(base_name(ob) != LIB_CONNECT) others += ({ ob });
        }
    }

    foreach(object thingy in others){
        if(!userp(thingy) && !environment(thingy) && 
          !inherits(LIB_SHADOW, thingy) &&
          member_array(base_name(thingy), ExemptArray) == -1 ) {
            thingy->eventDestruct();
        }
    }
}

mixed reap_list(){
    //
    // returns a list of cloned objects without an environment
    //
    string s1,s2,list;
    object *objects;
    object *clones;

    objects = objects();
    clones = ({});
    list = "";

    foreach(object thing in objects){
        if(sscanf(file_name(thing),"%s#%s",s1,s2) > 0) {
            if(!environment(thing)) clones += ({ thing });
        }
    }

    rm("/tmp/lost_object_list.txt");
    foreach(object clone in clones){
        write_file("/tmp/lost_object_list.txt", file_name(clone)+"\n");
    }
    //write("Total size of list: "+sizeof(clones)+" lost objects.");
    return this_player()->eventPage("/tmp/lost_object_list.txt");
}

