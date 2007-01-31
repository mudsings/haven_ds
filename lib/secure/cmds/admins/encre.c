/*    /secure/cmd *    from the Dead Souls LPC Library
 *    turns a player into a creator
 *    created by Descartes of Borg 950322
 */

#include <lib.h>
#include <privs.h>
#include <daemons.h>
#include <rooms.h>

inherit LIB_DAEMON;

string PlayerName;

mixed cmd(string args) {
    object ob, cre_ob, jeans, shirt, robe, hat, book, staff;
    string file, nom;

    if( !((int)master()->valid_apply(({ PRIV_ASSIST, PRIV_SECURE, LIB_CONNECT }))) )
	error("Illegal encre attempt: "+get_stack()+" "+identify(previous_object(-1)));

    if( args == "" || !stringp(args) ) 
	return "Who do you want to make a creator?";
    nom = convert_name(args);
    if( !user_exists(nom) ) return capitalize(nom) + " is not a member of " +
	possessive_noun(mud_name()) + " reality.";
    if( !strsrch(file = save_file(nom), DIR_CRES) )
	return "You cannot make "+capitalize(args)+" a creator.";

    if(!ob=find_player(nom)){
	if(member_array(nom, PLAYERS_D->GetPendingEncres()) != -1){
	    write("That person is already pending creatorship.");
	    return 1;
	}
	PLAYERS_D->RemovePendingDecre(lower_case(nom));
	PLAYERS_D->AddPendingEncre(lower_case(nom));
	write(capitalize(nom)+" will be a creator next time they log in.");
	return 1;
    }

    if( file_size(DIR_CRES+"/"+nom[0..0]) != -2) mkdir(DIR_CRES+"/"+nom[0..0]);
    if(rename(file+__SAVE_EXTENSION__, DIR_CRES+"/"+nom[0..0]+"/"+nom+__SAVE_EXTENSION__))
	return "You failed due to lack of write access to "+DIR_CRES+".";
    PLAYERS_D->eventCre(lower_case(nom));
    if( ob = find_player(nom) ) {
	ob->SetProperty("brand_spanking_new",0);
	PlayerName = nom;
	catch(cre_ob = (object)master()->player_object(nom));
	PlayerName = 0;
	if( !cre_ob ) {
	    message("system", "Failed to create a cre object.", this_player());
	    message("system", "Please log out and log back in.", ob);
	    return 1;
	}
	exec(cre_ob, ob);
	cre_ob->SetProperty("brand_spanking_new",0);
	cre_ob->Setup();
	ob->eventDestruct();
	message("system", "You are now a creator.", cre_ob);
	message("shout", (string)cre_ob->GetName() + " is now a creator!",
	  users(), ({ this_player(), cre_ob }));
	if( file_size(file+__SAVE_EXTENSION__) > -1 ) rm(file+__SAVE_EXTENSION__);
	make_workroom(cre_ob);
	cre_ob->eventForce("home");
	cre_ob->eventForce("cd");
	jeans = present("jeans",cre_ob);
	shirt = present("t-shirt",cre_ob);
	if(jeans) jeans->eventMove(ROOM_FURNACE);
	if(shirt) shirt->eventMove(ROOM_FURNACE);
	robe = new("/domains/default/armor/robe");
	if(robe) robe->eventMove(cre_ob);
	hat = new("/domains/default/armor/wizard_hat");
	if(hat) hat->eventMove(cre_ob);
	robe = present("robe",cre_ob);
	if(robe) cre_ob->eventForce("wear robe");
	hat  =  present("wizard hat",cre_ob);
	if(hat) cre_ob->eventForce("wear wizard hat");
	staff = new("/secure/obj/staff");
	if(staff) staff->eventMove(cre_ob);
	book = new("/domains/default/obj/manual");
	if(book && !present("manual",cre_ob))  book->eventMove(cre_ob);
	else if(book) book->eventMove(ROOM_FURNACE);
	cre_ob->AddChannel(({"cre", "newbie", "gossip", "ds", "ds_test", "lpuni", "death", "connections","intercre","dchat","inews" }));
	cre_ob->SetPolyglot(1);
	cre_ob->save_player((string)cre_ob->GetKeyName());
    }
    return 1;
}

string GetKeyName() { return PlayerName; }

void help() {
    message("help",
      "Syntax: encre <person>\n\n"
      "Makes the target a creator. If the target is not "
      "logged in, they will be made a creator when "
      "they next log in."
      "\n\n"
      "See also: decre, rid", this_player()
    );
}

