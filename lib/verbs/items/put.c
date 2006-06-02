/*    /verbs/items/put.c
 *    from the Dead Souls Object Library
 *    put OBJ in OBJ
 *    put OBJ into OBJ
 *    created by Descartes of Borg 950114
 *    Version: @(#) put.c 1.2@(#)
 *    Last modified: 96/12/31
 */

#include <lib.h>
#include "include/put.h"

inherit LIB_VERB;
string *eligible;

static void create() {
    verb::create();
    SetVerb("put");
    SetSynonyms("place", "stick");
    SetRules("OBS in OBJ", "OBS into OBJ", "OBS on OBJ", "OBS onto OBJ",
      "WRD WRD in OBJ", "WRD WRD into OBJ", "WRD WRD on OBJ", "WRD WRD onto OBJ");
    SetErrorMessage("Put what where?");
    SetHelp("Syntax: <put ITEM into CONTAINER>\n"
      "Syntax: <put ITEM onto SURFACE>\n\n"
      "Allows you to stick objects into other objects.\n\n"
      "Synonyms: place, stick\n\n"
      "See also: get, give, drop");
}


mixed can_put_obs_word_obj(object *foo1, string wrd, object foo2) {
    tc("1");
    tc("foo1: "+identify(foo1));
    tc("foo2: "+identify(foo2));
    if(check_light()) return this_player()->CanManipulate();
    else return 0;
}

mixed can_put_obj_word_obj(object target, string wrd, object storage) {
    object *target_arr;
    if(target) target_arr = ({ target });
    return can_put_obs_word_obj(target_arr, wrd, storage);
}

mixed do_put_obj_word_obj(object what, string wrd, object storage) {

    if(wrd == "in" || wrd == "into") return (mixed)storage->eventPutInto(this_player(), what);
    if(wrd == "on" || wrd == "onto") return (mixed)storage->eventPutOnto(this_player(), what);
}

mixed do_put_obs_word_obj(mixed *res, string wrd, object storage) {
    object *obs;


    tc("2");
    obs = filter(res, (: objectp :));

    //tc("a");
    if( !sizeof(obs) ) {
	mixed *ua;

	ua = unique_array(res, (: $1 :));
	//tc("b");
	foreach(string *lines in ua) {
	    if(storage && storage->GetClosed()) 
		write(capitalize(storage->GetShort())+" is closed.");
	    else write("That doesn't seem possible at the moment.");
	    return 1;
	}
	//tc("c");
	if(storage && storage->GetClosed()) 
	    write(capitalize(storage->GetShort())+" is closed.");
	else write("That doesn't seem possible at the moment.");
	return 1;
    }
    //tc("d");
    if(!sizeof(filter(obs, (: environment($1) == this_player() :)))){
	write("You don't seem to be in possession of that.");
	eligible = ({});
	//tc("1");
	return 1;
    }
    eligible=filter(obs, (: (!($1->GetWorn()) && environment($1) == this_player()) :)); 
    if(!sizeof(eligible)){
	write("Remove or unwield items before trying to put them somewhere.");
	eligible = ({});
	//tc("2");
	return 1;
    }
    //tc("eligible: "+identify(eligible));
    if(wrd == "in" || wrd == "into") {
	foreach(object ob in eligible)
	storage->eventPutInto(this_player(), ob);
    }
    if(wrd == "on" || wrd == "onto") {
	foreach(object ob in eligible)
	storage->eventPutOnto(this_player(), ob);
    }
    eligible = ({});
    return 1;
}

mixed can_put_wrd_wrd_word_obj(string num, string curr,string wrd, mixed container) {
    int amt;

    //tc("num: "+num);
    //tc("curr: "+curr);
    //tc("wrd: "+wrd);
    //tc("container: "+identify(container));
    if( !num || !curr ) return 0;
    if( (amt = to_int(num)) < 1 ) return "You cannot do that!";
    //tc("1","blue");
    if( (int)this_player()->GetCurrency(curr) < amt )
	return "You don't have that much " + curr + ".";
    //tc("2","green");
    if(this_player()->GetLevel() < 4) {
	write("Newbies cannot drop money.");
	return "Newbies can't drop money.";
    }
    if(wrd == "on" || wrd == "onto"){
	if(container && !inherits( LIB_SURFACE, container ) ) return "#That isn't a load-bearing surface.";
    }
    //tc("3","red");
    if(container && container->GetClosed()){
	return "#That's closed.";
    }
    //tc("hmm");
    //return this_player()->CanManipulate();
    if(intp(check_light())) return this_player()->CanManipulate();
    else return check_light();
}


mixed do_put_wrd_wrd_word_obj(string num, string curr, mixed wort, object ob) {
    object pile, env;
    int amt;
    //tc("ob: "+identify(ob),"yellow");
    //tc("wort: "+wort,"yellow");
    if(wort == "on") wort = "onto";
    if(wort == "in") wort = "into";
    //tc("wort: "+wort,"yellow");

    if(wort == "onto" && !inherits( LIB_SURFACE, ob ) ) {
	//tc("wtf1");
	write("That isn't a load-bearing surface.");
	return 1;
    }
    if(wort == "into" && inherits( LIB_SURFACE, ob ) ) {
	//tc("wtf2");
	write("That's a surface. Try \"put on\"");
	return 1;
    }


    amt = to_int(num);
    env = environment(this_player());
    pile = new(LIB_PILE);
    pile->SetPile(curr, amt);
    if( !((int)pile->eventMove(ob)) ||
      (int)this_player()->AddCurrency(curr, -amt) == -1 ) {
	this_player()->eventPrint("Something prevents your action.");
	pile->eventDestruct();
	return 1;
    }
    this_player()->eventPrint("You put " + amt + " " + curr + 
      " "+wort+" "+ob->GetShort()+".");

    environment(this_player())->eventPrint((string)this_player()->GetName() +
      " puts some " + curr + " "+wort+" "+ob->GetShort()+".",
      this_player());
    return 1;
}

mixed eventCheckLight(object who) {
    return check_light(who);
}
