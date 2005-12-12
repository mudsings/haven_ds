/*    /daemon/create.c
 *    From Dead Souls LPMud
 *    Mediates requests to create and modify objects
 */

#include <lib.h>
#include <dirs.h>
#include <daemons.h>
#include <modules.h>
#include <commands.h>
#include <rooms.h>

inherit LIB_DAEMON;

int eventDelete(object ob, string value);
string global1, global2;

string *base_arr = ({"SetLong","SetShort","SetItems","SetListen","SetSmell"});
string *item_arr = base_arr + ({"SetKeyName", "SetId", "SetMass","SetCost","SetValue","SetAdjectives","SetDamagePoints" });
string *storage_arr = item_arr + ({"SetMaxCarry","SetInventory", "SetCanClose", "SetCanLock","SetMaxRecurse","SetLocked","SetClosed","SetKey"});
string *room_arr = base_arr + ({"SetTown","SetNightLong","SetDayLong","SetClimate","SetAmbientLight","SetNightLight","SetDayLight","SetObviousExits", "SetInventory", "SetEnters"});
string *npc_arr = base_arr + ({"SetLevel","SetKeyName", "SetId", "SetLevel", "SetRace", "SetClass","SetGender", "SetInventory", "SetHealthPoints","SetMaxHealthPoints", "SetAdjectives"});
string *armor_arr = item_arr +({"SetRestrictLimbs","SetProtection","SetArmorType"});
string *weapon_arr = item_arr + ({"SetClass","SetWeaponType","SetDamageType","SetHands"});
string *chair_arr = item_arr + ({"SetMaxSitters","SetMaxCarry","SetInventory"});
string *bed_arr = chair_arr + ({"SetMaxLiers"});
string *table_arr = storage_arr + bed_arr;

string *all_arr = room_arr + npc_arr + armor_arr + weapon_arr + bed_arr;


static void create() {
    daemon::create();
    SetNoClean(1);
}

string GetSettings(string str){
    string ret;
    string *name = ({});
    ret = "";
    switch(str){
    case "room" : name = room_arr; break;
    case "npc" : name = npc_arr; break;
    case "armor" : name = armor_arr; break;
    case "weapon" : name = weapon_arr; break;
    case "item" : name = item_arr; break;
    default : name = room_arr;
    }
    foreach(string thing in name){
	ret += thing+", ";
    }
    ret = truncate(ret,2)+".";
    return ret;
}

mapping QueryMap(string str,object ob){
    switch(str){
    case "SetItems" : return ob->GetItemsMap();break;
    case "SetSmell" : return ob->GetSmellMap();break;
    case "SetListen" : return ob->GetListenMap();break;
    case "SetInventory" : return ob->GetInventory();break;
    case "SetEnters" : return ob->GetEnterMap();break;
    default : return ([]);
    }

}

mixed eventModify(object ob, string str){
    string inheritance, out, tmpfile, filename, mode, metamode;
    mixed value, mixed_tmp;
    int invalid, setmap;
    object furnace;
    object *players;
    mapping temp_map;
    string *p_array, *array_props;

    invalid = 1;
    filename = base_name(ob)+".c";
    tmpfile = generate_tmp(ob);

    if(!check_privs(this_player(),filename)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    if(!check_privs(this_player(),tmpfile)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    cp(filename, tmpfile);

    inheritance = "";
    if(inherits(LIB_ROOM,ob)) inheritance += " room";
    if(inherits(LIB_NPC,ob)) inheritance += " npc";
    if(inherits(LIB_WEAPON,ob)) inheritance += " weapon";
    if(inherits(LIB_ARMOR,ob)) inheritance += " armor";
    if(inherits(LIB_ITEM,ob)) inheritance += " item";
    if(inherits(LIB_STORAGE,ob)) inheritance += " storage";
    if(inherits(LIB_CHAIR,ob)) inheritance += " chair";
    if(inherits(LIB_BED,ob)) inheritance += " bed";
    if(inherits(LIB_TABLE,ob)) inheritance += " table";

    if(!inheritance || inheritance == ""){
	write("The object you want to modify lacks an init() function.");
	write("Please correct this by issuing the initfix command, then try again.");
	return 1;
    }
    //tc("ob is: "+identify(ob));
    //tc("LIB_ROOM is: "+LIB_ROOM);
    //tc("inherits: "+inheritance);

    //if(sscanf(str,"%s %s %d",mode, metamode, value) != 3) sscanf(str,"%s %s %s",mode, metamode, value);
    //if(!value){
    if(sscanf(str,"%s %d",mode, value) != 2) sscanf(str,"%s %s",mode, value); 
    //}
    if(!value) mode = str;
    if(!value) value = 0;

    if(mode == "delete") {
	mode = value;
	value = "delete";
    }

    setmap = 0;
    switch(lower_case(mode)){
    case "short" : out = "SetShort";break;
    case "setshort" : out = "SetShort";break;
    case "long" : out = "SetLong";break;
    case "setlong" : out = "SetLong";break;
    case "setnightlong" : out = "SetNightLong";break;
    case "nightlong" : out = "SetNightLong";break;
    case "daylong" : out = "SetDayLong";break;
    case "setdaylong" : out = "SetDayLong";break;
    case "climate" : out = "SetClimate";break;
    case "setclimate" : out = "SetClimate";break;
    case "light" : out = "SetAmbientLight";break;
    case "setlight" : out = "SetAmbientLight";break;
    case "nightlight" : out = "SetNightLight";break;
    case "setnightlight" : out = "SetNightLight";break;
    case "daylight" : out = "SetDayLight";break;
    case "setdaylight" : out = "SetDayLight";break;
    case "obvious" : out = "SetObviousExits";break;
    case "setobvious" : out = "SetObviousExits";break;
    case "obviousexits" : out = "SetObviousExits";break;
    case "setobviousexits" : out = "SetObviousExits";break;
    case "town" : out = "SetTown";break;
    case "settown" : out = "SetTown";break;
    case "keyname" : out = "SetKeyName";break;
    case "setkeyname" : out = "SetKeyName";break;
    case "name" : out = "SetKeyName";break;
    case "setname" : out = "SetKeyName";break;
    case "id" : out = "SetId";break;
    case "setid" : out = "SetId";break;
    case "class" : out = "SetClass";break;
    case "setclass" : out = "SetClass";break;
    case "maxhealth" : out = "SetMaxHealthPoints";break;
    case "maxhealthpoints" : out = "SetMaxHealthPoints";break;
    case "setmaxhealth" : out = "SetMaxHealthPoints";break;
    case "setmaxhealthpoints" : out = "SetMaxHealthPoints";break;
    case "level" : out = "SetLevel";break;
    case "setlevel" : out = "SetLevel";break;
    case "race" : out = "SetRace";break;
    case "setrace" : out = "SetRace";break;
    case "health" : out = "SetHealthPoints";break;
    case "healthpoints" : out = "SetHealthPoints";break;
    case "sethealth" : out = "SetHealthPoints";break;
    case "sethealthpoints" : out = "SetHealthPoints";break;
    case "gender" : out = "SetGender";break;
    case "setgender" : out = "SetGender";break;
    case "sex" : out = "SetGender";break;
    case "setsex" : out = "SetGender";break;
    case "mass" : out = "SetMass";break;
    case "setmass" : out = "SetMass";break;
    case "weight" : out = "SetMass";break;
    case "setweight" : out = "SetMass";break;
    case "cost" : out = "SetCost";break;
    case "setcost" : out = "SetCost";break;
    case "value" : out = "SetValue";break;
    case "setvalue" : out = "SetValue";break;
    case "dollarcost" : out = "SetDollarCost";break;
    case "setdollarcost" : out = "SetDollarCost";break;
    case "adjectives" : out = "SetAdjectives";break;
    case "setadjectives" : out = "SetAdjectives";break;
    case "adj" : out = "SetAdjectives";break;
    case "setadj" : out = "SetAdjectives";break;
    case "adjects" : out = "SetAdjectives";break;
    case "setadjects" : out = "SetAdjectives";break;
    case "adjs" : out = "SetAdjectives";break;
    case "setadjs" : out = "SetAdjectives";break;
    case "damage" : out = "SetDamagePoints";break;
    case "setdamage" : out = "SetDamagePoints";break;
    case "damagepoints" : out = "SetDamagePoints";break;
    case "setdamagepoints" : out = "SetDamagePoints";break;
    case "restrictlimbs" : out = "SetRestrictLimbs";break;
    case "setrestrictlimbs" : out = "SetRestrictLimbs";break;
    case "limbs" : out = "SetRestrictLimbs";break;
    case "setlimbs" : out = "SetRestrictLimbs";break;
    case "protection" : out = "SetProtection";break;
    case "setprotection" : out = "SetProtection";break;
    case "setprotections" : out = "SetProtection";break;
    case "protections" : out = "SetProtection";break;
    case "armortype" : out = "SetArmorType";break;
    case "setarmortype" : out = "SetArmorType";break;
    case "weapontype" : out = "SetWeaponType";break;
    case "setweapontype" : out = "SetWeaponType";break;
    case "damagetypes" : out = "SetDamageTypes";break;
    case "setdamagetypes" : out = "SetDamageTypes";break;
    case "hands" : out = "SetHands";break;
    case "sethands" : out = "SetHands";break;
    case "stuff" : out = "AddStuff";break;
    case "addstuff" : out = "AddStuff";break;
    case "setstuff" : out = "AddStuff";break;
    case "carry" : out = "SetMaxCarry";break;
    case "maxcarry" : out = "SetMaxCarry";break;
    case "setmaxcarry" : out = "SetMaxCarry";break;
    case "closed" : out = "SetClosed";break;
    case "setclosed" : out = "SetClosed";break;
    case "locked" : out = "SetLocked";break;
    case "setlocked" : out = "SetLocked";break;
    case "canclose" : out = "SetCanClose";break;
    case "setcanclose" : out = "SetCanClose";break;
    case "canlock" : out = "SetCanLock";break;
    case "setcanlock" : out = "SetCanLock";break;
    case "key" : out = "SetKey";break;
    case "setkey" : out = "SetKey";break;
    case "recurse" : out = "SetMaxRecurse";break;
    case "maxrecurse" : out = "SetMaxRecurse";break;
    case "setmaxrecurse" : out = "SetMaxRecurse";break;
    case "sitters" : out = "SetMaxSitters";break;
    case "maxsitters" : out = "SetMaxSitters";break;
    case "setmaxsitters" : out = "SetMaxSitters";break;
    case "liers" : out = "SetMaxLiers";break;
    case "maxliers" : out = "SetMaxLiers";break;
    case "setmaxliers" : out = "SetMaxLiers";break;
    case "item" : out = "SetItems";setmap = 1;break;
    case "setitem" : out = "SetItems";setmap = 1;break;
    case "items" : out = "SetItems";setmap = 1;break;
    case "setitems" : out = "SetItems";setmap = 1;break;
    case "inventory" : out = "SetInventory";setmap = 1;break;
    case "inv" : out = "SetInventory";setmap = 1;break;
    case "setinv" : out = "SetInventory";setmap = 1;break;
    case "setinventory" : out = "SetInventory";setmap = 1;break;
    case "smell" : out = "SetSmell";setmap = 1;break;
    case "setsmell" : out = "SetSmell";setmap = 1;break;
    case "listen" : out = "SetListen";setmap = 1;break;
    case "setlisten" : out = "SetListen";setmap = 1;break;
    case "enter" : out = "SetEnters";setmap = 1;break;
    case "enters" : out = "SetEnters";setmap = 1;break;
    case "setenters" : out = "SetEnters";setmap = 1;break;
    case "setenter" : out = "SetEnters";setmap = 1;break;
    default : out = mode;
    }

    if(value == "delete") {
	eventDelete(ob, out);
	rm(tmpfile);
	return 1;
    }

    if(grepp(inheritance,"room") && member_array(out,room_arr) != -1) invalid = 0;
    if(grepp(inheritance,"npc") && member_array(out,npc_arr) != -1) invalid = 0;
    if(grepp(inheritance,"weapon") && member_array(out,weapon_arr) != -1) invalid = 0;
    if(grepp(inheritance,"armor") && member_array(out,armor_arr) != -1) invalid = 0;
    if(grepp(inheritance,"storage") && member_array(out,storage_arr) != -1) invalid = 0;
    if(grepp(inheritance,"chair") && member_array(out,chair_arr) != -1) invalid = 0;
    if(grepp(inheritance,"bed") && member_array(out,bed_arr) != -1) invalid = 0;
    if(grepp(inheritance,"table") && member_array(out,table_arr) != -1) invalid = 0;
    else if(grepp(inheritance,"item") && member_array(out,item_arr) != -1) invalid = 0;

    if(invalid) {
	write("Invalid Property.");
	return 1;
    }

    if(out == "SetProtection"){
	load_object(MODULES_ARMOR)->eventStartQuestions(value,ob);
	return 1;
    }

    if(setmap == 1) {
	temp_map = QueryMap(out, ob);
	temp_map = load_object(MODULES_MAPPING)->eventStartQuestions(temp_map, ob, tmpfile, out);
	return 1;
    }

    array_props = ({"SetId","SetAdjectives","SetRestrictLimbs"});
    if(member_array(out,array_props) != -1){
	load_object(MODULES_GENERIC)->eventStartQuestions(ob, tmpfile, ({value}), out);
	return 1;
    }


    else {
	if(!value || value == "" ) {
	    write("This setting requires a value.");
	    return 1;
	}
	switch(out){
	case "SetLong" : p_array = ({"SetShort","SetAmbientLight","SetDayLight","SetNightLight","create()","create ()","create"}); break;
	case "SetDayLong" : p_array = ({"SetShort","SetAmbientLight","SetDayLight","SetNightLight","SetNightLong","SetLong","create()","create ()","create"}); break;
	case "SetNightLong" : p_array = ({"SetShort","SetAmbientLight","SetDayLight","SetNightLight","SetDayLong","SetLong","create()","create ()","create"}); break;
	case "SetShort" : p_array = ({"SetAmbientLight","SetDayLight","SetNightLight","create()","create ()","create"}); break;
	case "SetHealthPoints" : p_array = ({"SetInventory","SetMaxHealhPoints","SetClass","SetRace","SetLong"});break;
	case "SetMaxHealthPoints" : p_array = ({"SetInventory","SetHealhPoints","SetClass","SetRace","SetLong"});break;
	case "SetSmell" : p_array = ({"SetItems","SetInventory","SetListen","SetLong","SetDayLong","SetNightLong","SetShort"});break;
	case "SetListen" : p_array = ({"SetItems","SetInventory","SetSmell","SetLong","SetDayLong","SetNightLong","SetShort"});break;
	case "SetItems" : p_array = ({"SetAmbientLight","SetDayLight","SetNightLight","create()","create ()","SetShort","SetLong","SetDayLong","SetNightLong"});break;
	case "SetAmbientLight" : p_array = ({"SetDayLight","SetNightLight","create()","create ()","SetShort","SetLong","SetDayLong","SetNightLong"});break;
	case "SetDayLight" : p_array = ({"SetAmbientLight","SetNightLight","create()","create ()","SetShort","SetLong","SetDayLong","SetNightLong"});break;
	case "SetNightLight" : p_array = ({"SetAmbientLight","SetDayLight","create()","create ()","SetShort","SetLong","SetDayLong","SetNightLong"});break;
	default : p_array = ({"SetItems","SetLong","SetDayLong","SetNightLong","SetShort"});
	}

	load_object(MODULES_FILE)->eventModString(tmpfile, out, value, p_array);
    }
    mixed_tmp = load_object("/secure/cmds/creators/update")->cmd("-a "+tmpfile);
    if(!mixed_tmp || !intp(mixed_tmp)) {
	write("This would screw up your file. Aborting modification.");
	return 1;
    }
    load_object(MODULES_GENERIC)->eventGeneralStuff(tmpfile);
    if(!(cp(tmpfile,filename))) {
	write("You do not have write access to that file. Aborting modification.");
	return 1;
    }
    players = get_livings(environment(this_player()),1);
    load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
    furnace = load_object(ROOM_FURNACE);
    if(inheritance == "room") {
	players->eventMove(load_object(filename));
    }
    else ob->eventMove(furnace);
    new(filename)->eventMove(environment(this_player()));

    rm(tmpfile);
    return CREATE_D+" Done.";
}

int eventDelete(object ob, string value){
    string ret, tmpfile, filename;
    mixed mixed_tmp;
    object *players;

    filename = base_name(ob)+".c";
    tmpfile = generate_tmp(filename);

    if(!check_privs(this_player(),filename)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    if(!check_privs(this_player(),tmpfile)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    cp(filename,tmpfile);
    mixed_tmp = load_object("/secure/cmds/creators/update")->cmd("-a "+tmpfile);
    if(!mixed_tmp || !intp(mixed_tmp)) {
	write("Target file is screwed up. Aborting delete.");
	return 1;
    }

    ret = remove_matching_line(read_file(tmpfile),value,1);
    write_file(tmpfile,ret,1);
    mixed_tmp = load_object("/secure/cmds/creators/update")->cmd("-a "+tmpfile);
    if(!mixed_tmp || !intp(mixed_tmp)) {
	write("This change would screw up your file. Aborting delete.");
	return 1;
    }
    if(inherits(LIB_ROOM,load_object(filename))){
	cp(tmpfile,filename);
	players = get_livings(environment(this_player()),1);
	load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
	players->eventMoveLiving(load_object(filename));
    }
    else {
	cp(tmpfile,filename);
	ob->eventMove(load_object(ROOM_FURNACE));
	load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
	new(filename)->eventMove(environment(this_player()));
    }
    rm(tmpfile);
    write("Setting deleted.");
    return 1;
}
int eventResumeArrayMod(object target, string tmpfile, string *NewArr, string func){
    string filename, ret, array_string;
    string *p_array;
    mixed mx;
    filename = base_name(target)+".c";
    if(!check_privs(this_player(),filename)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }
    if(!cp(filename, tmpfile)) {
	write("oops! You don't have read access to that file. So sorry.");
	return 1;
    }
    array_string = func + "( ({";
    foreach(string foo in NewArr){
	array_string += "\""+foo+"\", ";
    }
    array_string = truncate(array_string, 2);
    array_string +=  "}) );";
    switch(func){
    case "SetId" : p_array = ({"SetName","SetKeyName","create()","create ()","SetAdjectives"});break;
    case "SetAdjectives" : p_array = ({"SetName","SetKeyName","create()","create ()","SetId"});break;
    case "SetRestrictLimbs" : p_array = ({ "SetArmorType","SetDamagePoints","SetMass"});break;
    default : p_array = ({"SetLong","SetShort","SetDayLong","SetNightLong"});
    }
    ret = remove_matching_line(read_file(tmpfile),func);
    ret = load_object(MODULES_FILE)->eventAppend(ret,p_array,"\n"+array_string+"\n");
    write_file(tmpfile,ret,1);
    load_object(MODULES_GENERIC)->eventGeneralStuff(tmpfile);
    catch( mx = load_object(CMD_UPDATE)->cmd(tmpfile) );
    if(!mx || !intp(mx)){
	write("This change would screw up the object. Aborting.");
	return 1;
    }
    if(!cp(tmpfile, filename)){
	write("You don't have write permission to that. Sorry.");
	return 1;
    }
    load_object(CMD_UPDATE)->cmd(filename);
    if(inherits(LIB_ROOM,load_object(filename))) this_player()->eventMoveLiving(filename);    else{
	target->eventMove(load_object(ROOM_FURNACE));
	new(filename)->eventMove(environment(this_player()));
    }
    write(func+" modification complete.");
    rm(tmpfile);
    return 1;
}


int eventResumeMappingChange(object target, string tmpfile, mapping NewMap, string func){
    string map_string, filename, ret;
    mixed mx;
    string *p_array;
    map_string = load_object(MODULES_MAPPING)->eventStringifyMap(NewMap);
    map_string = func+"("+map_string+");";
    filename = base_name(target)+".c";
    write_file("/tmp/map.txt",map_string,1);

    if(!check_privs(this_player(),filename)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    if(!cp(filename, tmpfile)) {
	write("oops! You don't have read access to that file. So sorry.");
	return 1;
    }

    ret = remove_matching_line(read_file(tmpfile),func);
    switch(func){
    case "SetHealthPoints" : p_array = ({"SetInventory","SetMaxHealhPoints","SetClass","SetRace","SetLong"});break;
    case "SetMaxHealthPoints" : p_array = ({"SetInventory","SetHealhPoints","SetClass","SetRace","SetLong"});break;
    case "SetSmell" : p_array = ({"SetItems","SetInventory","SetListen","SetLong","SetDayLong","SetNightLong","SetShort"});break;
    case "SetListen" : p_array = ({"SetItems","SetInventory","SetSmell","SetLong","SetDayLong","SetNightLong","SetShort"});break;
    default : p_array = ({"SetItems","SetLong","SetDayLong","SetNightLong","SetShort"});
    }
    ret = load_object(MODULES_FILE)->eventAppend(ret,p_array,"\n"+map_string+"\n");
    global1 = tmpfile;
    global2 = ret;
    unguarded( (: write_file(global1,global2,1) :) );
    catch(mx = load_object(CMD_UPDATE)->cmd("-a "+tmpfile));
    if(!mx || !intp(mx)){
	write("This change would screw up the object. Aborting.");
	return 1;
    }
    load_object(MODULES_GENERIC)->eventGeneralStuff(tmpfile);
    if(!cp(tmpfile, filename)){
	write("You don't have write permission to that. Sorry.");
	return 1;
    }
    load_object(CMD_UPDATE)->cmd(filename);
    if(inherits(LIB_ROOM,load_object(filename))) this_player()->eventMoveLiving(filename);
    else{
	target->eventMove(load_object(ROOM_FURNACE));
	new(filename)->eventMove(environment(this_player()));
    }
    write(func+" modification complete.");
    rm(tmpfile);
    return 1;
}

