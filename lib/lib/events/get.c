/*    /lib/get.c
 *    from the Dead Souls Object Library
 *    handles the picking up of objects
 *    created by Descartes of Borg 960114
 *    Version: @(#) get.c 1.3@(#)
 *    Last Modified: 96/12/21
 */

private mixed PreventGet;

// abstract methods
int GetMass();
mixed GetProperty(string key);
string GetShort();
string GetKeyName();
mixed eventMove(mixed dest);
// end abstract methods

mixed GetPreventGet(){
    return PreventGet;
}

mixed SetPreventGet(mixed val){
    return (PreventGet = val);
}

mixed CanGet(object who){
    if( !((int)who->CanCarry(GetMass())) ) return "It is too heavy for you!";
    if( !PreventGet && !GetProperty("keep") ) return 1;
    if( stringp(GetProperty("keep")) ){
        if( (string)who->GetKeyName() == GetProperty("keep") ){
            if( !PreventGet ) return 1;
        }
        else return "Mystical forces prevent you from getting " + this_object()->GetShort() + ".";
    }
    if( intp(PreventGet) ) return 0;
    if( stringp(PreventGet) ) return PreventGet;
    if( objectp(PreventGet) ){
        if( PreventGet == who )
            return capitalize(this_object()->GetShort()) + " simply will not be taken.";
        else return 1;
    }
    else{
        return evaluate(PreventGet, who);
    }
}

mixed eventGet(object who){
    if( !eventMove(who) ){
        who->eventPrint("You fail to get it.");
        return 1;
    }
    who->eventPrint("You get " + this_object()->GetShort() + ".");
    environment(who)->eventPrint((string)who->GetName() + " gets " +
      this_object()->GetShort() + ".", who);
    return 1;
}

static void create(){
    PreventGet = 0;
}

mixed direct_get_obj(object target){
    if(environment() == this_player())
        return "#You're already holding it.";
    if( environment() != environment(this_player()) ){
        string str = this_object()->GetShort();

        if( !str ) str = "It";
        else str = capitalize(str);
        return "#You may need to get closer to it. Perhaps \"get "+this_object()->GetKeyName()+
        " from\" something?";
    }
    return CanGet(this_player());
}

mixed direct_get_obj_out_of_obj(object target, object src){
    object env;
    string str;

    env = environment();
    if( !(str = this_object()->GetShort()) ) str = "It";
    else str = capitalize(str);
    if( env==this_player() || env ==environment(this_player()) || living(env) ){
        return "#You can't do that right now.";
    }
    return CanGet(this_player());
}

mixed direct_get_obj_from_obj(object target, object src){
    return direct_get_obj_out_of_obj(target, src);
}

mixed direct_get_obj_obj(object target, object src){
    return direct_get_obj_out_of_obj(target, src);
}
