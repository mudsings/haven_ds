#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_INSTALL;

void create(){
    ::create();
    SetKeyName("blue vial");
    SetId( ({"vial"}) );
    SetAdjectives( ({"small","transparent","blue","glowing"}) );
    SetShort("a glowing blue vial");
    SetLong("A small transparent vial filled with a glowing blue fluid.");
    SetMass(10);
    SetBaseCost("silver",1000);
    SetMaxClones(3);
    SetNoCondition(1);
    SetVendorType(VT_TREASURE);
}

varargs mixed eventInstall(object what, object where, int auto){
    int ret = this_object()->eventMove(where);
    if(ret){
        write("You install "+GetShort()+" into "+where->GetShort()+".");
        say(this_player()->GetName()+" installs "+GetShort()+" into "+
                possessive(this_player())+" "+where->GetShort()+".");
        return 1;
    }
    else {
        write("You fail to install it.");
        return 0;
    }
}

varargs mixed eventUninstall(object what, mixed auto){
    int success;
    object env = environment(this_object());
    if(!env || base_name(env) != "/domains/default/obj/generator"){
        write("It's not installed.");
        return 0;
    }
    if(!environment(env) || environment(env) != this_player() ||
            !environment(this_player()) || 
            environment(env) !=  environment(this_player())){
        write("That's not yours to uninstall.");
        return 0;
    }
    success = this_object()->eventMove(this_player());
    if(!success){
        write("You fail to uninstall it.");
        return 0;
    }
    else {
        write("You uninstall "+GetShort()+" from "+env->GetShort()+".");
        say(this_player()->GetName()+" uninstalls "+GetShort()+" from "+
                possessive(this_player())+" "+env->GetShort()+".");
        return 1;
    }
}

void init(){
    ::init();
}
