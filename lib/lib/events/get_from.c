/*    /lib/events/get_from.c
 *    From the Dead Souls Object Library
 *    Handles get from/put in
 *    Created by Descartes of Borg 961221
 *    Version: @(#) get_from.c 1.1@(#)
 *    Last modified: 96/12/22
 */

#include <lib.h>

// abstract methods
int AddCarriedMass(int amount);
string GetShort();
// end abstract methods

mixed CanGetFrom(object who, object item) {

    if( !item ) {
	return 0;
    }
    if( environment(item) != this_object() ) {
	return 0;
    }
    if( environment(item) != this_object() ) {
	item = present(item->GetKeyName(),this_object());
	if(!item) return 0;
    }

    if( (environment() != environment(this_player())) &&
      (environment() != this_player()) ) {
	return "#" + capitalize(GetShort()) + " is not in reach.";
    }
    return true(who);
}

mixed CanPutInto(object who, object item) {
    object env;

    if((inherits(LIB_SIT,item) && sizeof(item->GetSitters())) ||
      (inherits(LIB_LIE,item) && sizeof(item->GetLiers()))){
	write("There appears to be someone in your way.");
	return 0;
    }

    if( item == this_object() ) {
	return "#You cannot change the laws of physics.";
    }
    env = environment();
    if( env != this_player() && env != environment(this_player()) ) {
	return "#It is not within reach.";
    }

    if( this_object()->GetClosed() ){
	return "#It's closed!";
    }
    return true(who);
}

mixed CanPutOnto(object who, object item) {
    object env;

    if((inherits(LIB_SIT,item) && sizeof(item->GetSitters())) ||
      (inherits(LIB_LIE,item) && sizeof(item->GetLiers()))){
	write("There appears to be someone preventing your access.");
	return false(who);
    }

    if(!inherits( LIB_SURFACE, item ) ){
	return "#That isn't a load-bearing surface.";
    }
    if( item == this_object() ) {
	return "#You cannot change the laws of physics.";
    }
    env = environment();
    if( env != this_player() && env != environment(this_player()) ) {
	return "#It is not within reach.";
    }
    return 1;
}


mixed eventGetFrom(object who, object array what) {
    object array fin = ({});
    string array shorts;
    mapping mp = ([]);
    string msg;
    int i, maxi;

    if((inherits(LIB_SIT,this_object()) && sizeof(this_object()->GetSitters())) || 
      (inherits(LIB_LIE,this_object()) && sizeof(this_object()->GetLiers()))){
	write("There appears to be someone on there.");
	return 0;
    }

    foreach(object ob in what ) {
	if( environment(ob) != this_object() ) {
	    continue;
	}
	if( ob->CanGet(who) != 1 ) {
	    write("It would appear you can't get "+ob->GetShort()+" right now.");
	    continue;
	}
	if( !who->CanCarry(ob->GetMass()) ) {
	    write("It seems you can't carry it.");
	    continue;
	}
	if( !ob->eventMove(who) ) {
	    who->eventPrint("You have a problem getting " +
	      ob->GetShort() + ".");
	    continue;
	}
	AddCarriedMass( -(ob->GetMass()) );
	fin += ({ ob });
    }
    what = fin;
    if( !(maxi = sizeof(what)) ) {
	return 0;
    }
    shorts = map(what, (: (string)$1->GetShort() :));
    for(i=0; i<maxi; i++) {
	mp[shorts[i]]++;
    }
    maxi = sizeof(shorts = keys(mp));
    for(i=0, msg = ""; i<maxi; i++) {
	if( mp[shorts[i]] < 2 ) {
	    msg += shorts[i] + "%^RESET%^";
	}
	else {
	    msg += consolidate(mp[shorts[i]], shorts[i]) +  "%^RESET%^";
	}
	if( i == maxi-2 ) {
	    msg += ", and ";
	}
	else if( i != maxi-1 ) {
	    msg += ", ";
	}
    }
    send_messages("get", "$agent_name $agent_verb " + msg +
      " from $target_name.", who, this_object(), environment(who));
    return 1;
}

mixed eventPutInto(object who, object what) {
    return what->eventPut(who, this_object());
}

mixed eventPutOnto(object who, object what) {
    if((inherits(LIB_SIT,this_object()) && sizeof(this_object()->GetSitters())) ||
      (inherits(LIB_LIE,this_object()) && sizeof(this_object()->GetLiers()))){
	write("There appears to be someone in the way of that.");
	return 0;
    }
    return what->eventPut(who, this_object()," onto ");
}


int inventory_accessible() {
    return 1;
}

int inventory_visible() {
    return 1;
}

mixed indirect_get_obj_from_obj(object item, object container) {
    if(!item){
	return 0;
    }

    if(environment(item) != this_object()) return false(container);

    return CanGetFrom(this_player(), item);
}

mixed indirect_get_obj_out_of_obj(object item, object container) {
    return indirect_get_obj_from_obj(item, container);
}

mixed indirect_get_obs_from_obj(object array items, object storage) {
    if( !items ) {
	return (storage == this_object());
    }
    return 1;
}

mixed indirect_get_obs_out_of_obj(object array items, object storage) {
    return indirect_get_obs_from_obj(items, storage);
}

mixed indirect_put_obj_word_obj(object what, string word, object storage) {
    true(word);
    if( !what ) {
	return (storage == this_object());
    }
    return CanPutInto(this_player(), what);
}

mixed indirect_put_obs_word_obj(object *items, string wrd, object storage) {
    if( !items ) {
	return (storage == this_object());
    }
    return true(wrd);
}
