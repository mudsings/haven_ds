#include <lib.h>
#include <modules.h>

string globalstr, globalstr2, globaltmp;

varargs int eventCreateExit(string dir, string room, string file, int remote);
int eventRemoveExit(string dir, string filename);
varargs mixed eventProcessExits(string filename, string dir, string location);
varargs mixed eventProcessEnters(string filename, string dir, string location, object room);
string eventCopyRoom(string source, string dest);

string *cardinal_dirs = ( ({"north","south","east","west", "northeast","northwest","southeast","southwest","up","down", "out"}) );

mixed make(string str) {
    int enter;
    string *dir_array;
    string *exits;
    string *enters;
    string foo, current_dir, current_room, this_room, new_room, room_dir;
    string new_file, arg1, arg2, s1, s2;
    object room;

    if(!str || str == "") {
	write("You'll need to be more specific. Try 'help create'");
	return 1;
    }


    foo = ""+time();
    room = environment(this_player());
    current_dir = this_player()->query_cwd();
    current_room = base_name(room);
    dir_array = explode(current_room, "/");
    dir_array -= ({ dir_array[sizeof(dir_array) - 1] });
    room_dir = "/"+implode(dir_array,"/");


    if(file_exists(current_room+".c") && !check_privs(this_player(),current_room+".c")){
	write("You do not appear to have access to this room file. Modification aborted.");
	return 1;
    }

    if(!write_file(room_dir+"/"+foo+".foo","",1)) {
	write("You do not have write privileges to this area.");
	return 1;
    }

    rm(room_dir+"/"+foo+".foo");
    write("It appears you have write access to this area.");
    this_room = read_file(current_room+".c");
    new_room = read_file("/obj/room.c");


    if(sscanf(str," enter %s %s", s1, s2) == 2){ 
	arg1 = s1;
	arg2 = s2;
	enter = 1;
    }
    else if(sscanf(str," %s %s", s1, s2) == 2){ 
	arg1 = s1;
	arg2 = s2;
	enter = 0;
    }
    else {
	write("Usage: create room <direction> <file>");
	return 1;
    }

    exits = room->GetExits();
    enters = room->GetEnters();

    if(member_array(arg1,exits) != -1){
	write("This room already has an exit in that direction.");
	return 1;
    }

    if(member_array(arg1,enters) != -1){
	write("This room already has an enter by that name.");
	return 1;
    }

    if(strsrch(arg2,".c") == -1) arg2 += ".c";
    if(strsrch(arg2," ") != -1) arg2 = replace_string(arg2," ","");

    if(file_exists(arg2)) new_file = arg2;
    else{
	if(strsrch(arg2,"/") != -1) arg2 = replace_string(arg2,"/","");
	new_file = room_dir +"/"+ arg2;
    }


    if(new_file[0..7] == "/realms/" && strsrch(new_file,"/area/") != -1){
	if(file_size(new_file) < 0) cp("/obj/area_room.c",new_file);
    }
    else if(file_size(new_file) < 0) cp("/obj/room.c",new_file);

    eventCreateExit(arg1, current_room+".c", new_file);
    if(file_exists(new_file) &&
      grepp(read_file(new_file),"SetShort(\"a blank room\");") ) {
	eventCopyRoom(current_room+".c", new_file);
	load_object("/secure/cmds/creators/update")->cmd("-a "+new_file);
    }
    //write("You mutter an incantation and a new room appears!");
    return 1;
}

varargs int eventCreateExit(string dir, string room, string file, int remote){
    string this_room, param, line, newfile;
    string *file_arr;
    object *players;

    if(member_array(dir,cardinal_dirs) == -1) {
	this_object()->eventCreateEnter(dir, room, file, remote);
	return 1;
    }
    if(file_exists(room) && !check_privs(this_player(),room)){
	write("You do not appear to have access to this room file. Modification aborted.");
	return 1;
    }

    globalstr = room;
    unguarded( (: globaltmp = read_file(globalstr) :) );
    file_arr = explode(globaltmp,"\n");

    if(remote && member_array(dir,load_object(room)->GetExits()) != -1) return 0;

    if(strsrch(globaltmp,"SetExits") != -1) param = "SetExits";
    else if(strsrch(globaltmp,"AddExit") != -1) param = "AddExit";
    else if(strsrch(globaltmp,"SetLong") != -1) param = "SetLong";
    else {
	write("The room you are in is screwed up. Creation process halted.");
    }
    globaltmp = remove_matching_line(globaltmp,"SetObviousExits",1);
    globaltmp = remove_matching_line(globaltmp,"//extras",1);
    ////tc("globalstr: "+globalstr);
    //write("globaltmp: "+globaltmp);
    unguarded( (: write_file(globalstr,globaltmp,1) :) );
    players = get_livings(environment(this_player()),1);
    load_object("/secure/cmds/creators/update")->cmd("-a "+room);
    eventProcessExits(room, dir, file);

    if(!remote) {
	//call_out( (: eventCreateExit, opposite_dir(dir), file, room, 1 :), 1 );
	eventCreateExit(opposite_dir(dir), file, room, 1 );
	players->eventMove(load_object(room));
	write("You begin uttering a magical incantation.");
	say(this_player()->GetCapName()+" begins uttering a magical incantation.");
	this_object()->eventGeneralStuff(room);
    }
    this_object()->eventAddInit(room);
    if(remote){
	this_object()->eventGeneralStuff(room);
	write("You wave your hand, and a new exit appears.");
	say(this_player()->GetCapName()+" waves "+possessive(this_player())+" hand and a new exit appears.");
    }
    return 1;
}

int eventRemoveExit(string dir, string filename){
    string contents, search_str, map_str, key, val, new_file, tmpfile;
    string *file_arr;
    object *players;
    mapping PointlessMap;
    mapping ExitsMap = load_object(filename)->GetExitMap();

    //if(!sizeof(ExitsMap)) tc("Exitsmap is nothing");
    //else tc("ExitsMap: "+identify(ExitsMap));

    if(file_exists(filename) && !check_privs(this_player(),filename)){
	write("You do not appear to have access to this room's file. Modification aborted.");
	return 1;
    }

    tmpfile = generate_tmp(load_object(filename));
    //tmpfile = generate_tmp();
    globalstr = filename;
    globaltmp = tmpfile;
    unguarded( (: globalstr2 = read_file(globalstr) :) );
    contents = globalstr2;
    //tc("filename: "+filename);
    //tc("contents: "+contents);
    PointlessMap = ([]);

    PointlessMap = load_object(filename)->GetFullExitData();

    //if(!sizeof(PointlessMap)) write("PointlessMap is nothing");
    // else write("PointlessMap: "+identify(PointlessMap));

    //if(file_exists(filename)) tc(filename+" exists.");
    // else tc(filename+" doesn't exist.");

    if( member_array(dir,load_object(filename)->GetExits()) != -1 
      && !functionp(PointlessMap[dir]["pre"]) 
      && !functionp(PointlessMap[dir]["post"]) ) {
	this_object()->eventReadMapping(filename,({"SetExits"}), 1);
	//write("ExitsMap: "+identify(ExitsMap));
	map_delete(ExitsMap,dir);
	//tc("ExitsMap: "+identify(ExitsMap));
	map_str = "SetExits( ([ \n";
	foreach( key, val in ExitsMap){
	    if(!functionp(PointlessMap[key]["pre"]) 
	      && !functionp(PointlessMap[key]["post"])) map_str += "\""+key+"\" : \""+val+"\",\n";
	}
	map_str += "]) );";
	//write("map_str: "+map_str);

	unguarded( (: globalstr2 = read_file(globalstr) :) );
	contents = globalstr2;

	new_file = remove_matching_line(contents, "SetExits", 1); 
	new_file = this_object()->eventAppend(new_file,({"SetLong"}),"\n"+map_str+"\n");
	new_file = remove_matching_line(new_file,"SetObviousExits");
	new_file = remove_matching_line(new_file,"//extras");
	new_file = remove_matching_line(new_file,"AddExit(", 1, ":)");
	//write("new_file: "+new_file);
	globalstr2 = new_file;
	unguarded( (: write_file(globaltmp,globalstr2,1) :) );
	this_object()->eventGeneralStuff(tmpfile);
	globalstr = tmpfile;
	globalstr2 = filename;
	unguarded( (: cp(globalstr, globalstr2) :) );
	players = get_livings(environment(this_player()),1);
	load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
	players->eventMove(load_object(filename));
	//rm(tmpfile);
	write("With a puff of smoke, an exit vanishes!");
	return 1;
    }


    if(member_array(dir,load_object(filename)->GetExits()) != -1){
	globalstr = filename;
	new_file = read_file(filename);
	file_arr = explode(read_file(filename),"\n");
	foreach(string linea in file_arr){
	    if(strsrch(linea,"AddExit") != -1 && strsrch(linea,dir) != -1) {
		search_str = linea;
	    }
	}
	globalstr = tmpfile;
	globalstr2 = filename;
	globaltmp = remove_matching_line(new_file, search_str);
	unguarded( (: write_file(globalstr,globaltmp,1) :) );
	unguarded( (: cp(globalstr, globalstr2) :) );
	players = get_livings(environment(this_player()),1);
	load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
	eventProcessExits(filename);
	players->eventMove(load_object(filename));
	rm(tmpfile);
	write("With a puff of smoke, an exit vanishes!");
	return 1;
    }

    write("This room's SetExits does not contain that direction.");
    return 1;
}

varargs mixed eventProcessExits(string filename, string dir, string location){
    string tmpexit, search_str, map_str, key, val, new_file;
    string *file_arr;
    object *players;
    mapping PointlessMap;

    mapping ExitsMap = load_object(filename)->GetExitMap();

    if(file_exists(filename) && !check_privs(this_player(),filename)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    PointlessMap =  load_object(filename)->GetFullExitData();
    map_str = "SetExits( ([\n";

    this_object()->eventReadMapping(filename,({"SetExits"}), 1);
    new_file = remove_matching_line(read_file(filename),"SetObviousExits");
    new_file = remove_matching_line(new_file,"//extras");
    new_file = remove_matching_line(new_file, "AddExit(", 1, ":)");

    foreach( key, val in ExitsMap){
	if(!functionp(PointlessMap[key]["pre"]) &&
	  !functionp(PointlessMap[key]["post"])) map_str += "\""+key+"\" : \""+val+"\",\n";
    }
    if(dir && location && member_array(dir,load_object(filename)->GetExits()) == -1){
	map_str += "\""+dir+"\" : \""+location+"\",\n";
    }
    map_str += "]) );";

    globalstr = filename; 
    globaltmp = generate_tmp();

    globalstr2 = this_object()->eventAppend(new_file,({"SetLong","SetDayLong","SetNightLong","SetShort","create()","create"}),"\n"+map_str+"\n");
    unguarded( (: write_file(globaltmp,globalstr2,1) :) );
    unguarded( (: cp(globaltmp,globalstr) :) );
    load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
    rm(globaltmp);
    return 1;
}

string eventCopyRoom(string source, string dest){
    string homedir, areadir, tmpsource, map_str, new_file, foo;
    mapping DestExits;
    //write("source: "+source);
    //write("dest: "+dest);

    if(file_exists(source) && !check_privs(this_player(),source)){
	write("You do not appear to have access to this file. Modification aborted.");
	return "";
    }

    if(file_exists(dest) && !check_privs(this_player(),dest)){
	write("You do not appear to have access to that file. Modification aborted.");
	return "";
    }

    tmpsource = generate_tmp(source);
    globalstr2 = source;
    globalstr = tmpsource;
    unguarded( (: cp(globalstr2, globalstr) :) );
    globalstr2 = dest;
    if(!file_exists(source)) return "Source read failed.";
    if(!file_exists(tmpsource)) return "Read failed.";
    if(!file_exists(dest)) return "Destination read failed.";
    homedir = "/realms/"+this_player()->GetKeyName();
    areadir = homedir+"/area";

    unguarded( (: this_object()->eventReadMapping(globalstr,({"SetExits"}),1) :) ); 
    unguarded( (: this_object()->eventReadMapping(globalstr2,({"SetExits"}),1) :) ); 
    DestExits = load_object(dest)->GetExitMap();
    //tc("\n--------\n");

    unguarded( (: globaltmp = read_file(globalstr) :) ); 
    //write("globaltmp: "+globaltmp);
    //tc("\n--------\n");

    //write("DestExitS: "+identify(DestExits));
    map_str = "SetExits("+this_object()->eventStringifyMap(DestExits)+");\n";
    new_file = this_object()->eventAppend(globaltmp,({"SetLong"}),"\n"+map_str+"\n");
    new_file = replace_line(new_file,({"SetShort(",");"}),"SetShort(\"copy of "+last_string_element(source,"/")+"\");");
    replace_string(new_file,"\n\n\n","\n\n");

    //tc("new_file: "+new_file);
    //tc("\n--------\n");
    write_file(tmpsource,new_file,1);

    this_object()->eventGeneralStuff(tmpsource);
    globalstr = tmpsource;
    globalstr2 = dest;

    if(!unguarded( (: cp(globalstr, globalstr2) :) )){
	return "Write failed.";
    }

    this_object()->eventAddInit(dest);
    rm(tmpsource);
    return "Room data copy complete.";
}

varargs int eventCreateEnter(string dir, string room, string file, int remote){
    string this_room, line, newfile;
    object *players;

    if(file_exists(room) && !check_privs(this_player(),room)){
	write("You do not appear to have access to this room file. Modification aborted.");
	return 1;
    }

    if(!present(dir,environment(this_player()))){ 
	write("This room needs a corresponding SetItem to make a SetEnter ");
	write("of "+dir);
	return 1;
    }

    globalstr = room;
    unguarded( (: globaltmp = read_file(globalstr) :) );

    if(remote && member_array("out",load_object(room)->GetExits()) != -1) return 0;

    globaltmp = remove_matching_line(globaltmp,"SetObviousEnters",1);
    globaltmp = remove_matching_line(globaltmp,"//extras",1);
    //tc("globalstr: "+globalstr);
    //write("globaltmp: "+globaltmp);
    unguarded( (: write_file(globalstr,globaltmp,1) :) );
    players = get_livings(environment(this_player()),1);
    load_object("/secure/cmds/creators/update")->cmd("-a "+room);

    if(!remote) {
	//call_out( (: this_object()->eventCreateExit, "out", file, room, 1 :), 1 );
	eventProcessEnters(room, dir, file);
	this_object()->eventCreateExit("out", file, room, 1 );
	players->eventMove(load_object(room));
	say(this_player()->GetCapName()+" waves "+possessive(this_player())+" hand and a new enter appears.");
	this_object()->eventGeneralStuff(room);
    }
    this_object()->eventAddInit(room);
    if(remote){
	this_object()->eventGeneralStuff(room);
	write("You begin uttering a magical incantation.");
	write("You wave your hand, and a new enter appears.");
	say(this_player()->GetCapName()+" begins uttering a magical incantation.");
    }
    return 1;
}

int eventRemoveEnter(string dir, string filename){
    string contents, search_str, map_str, new_file, tmpfile;
    mixed *key;
    mixed val;
    string *file_arr;
    mixed *key_arr;
    object *players;
    mapping PointlessMap;
    mapping EntersMap = load_object(filename)->GetEnterMap();

    //if(!sizeof(EntersMap)) tc("Entersmap is nothing");
    //else tc("EntersMap: "+identify(EntersMap));

    if(file_exists(filename) && !check_privs(this_player(),filename)){
	write("You do not appear to have access to this room's file. Modification aborted.");
	return 1;
    }

    tmpfile = generate_tmp(load_object(filename));
    //tmpfile = generate_tmp();
    globalstr = filename;
    globaltmp = tmpfile;
    unguarded( (: globalstr2 = read_file(globalstr) :) );
    contents = globalstr2;
    //tc("filename: "+filename);
    //tc("contents: "+contents);
    PointlessMap = ([]);

    PointlessMap = load_object(filename)->GetEnterMap();

    //if(!sizeof(PointlessMap)) write("PointlessMap is nothing");
    // else write("PointlessMap: "+identify(PointlessMap));

    //if(file_exists(filename)) tc(filename+" exists.");
    // else tc(filename+" doesn't exist.");

    if( member_array(dir,load_object(filename)->GetEnters()) != -1) { 
	//&& !functionp(PointlessMap[dir]["pre"]) ) {
	this_object()->eventReadMapping(filename,({"SetEnters"}), 1);
	//write("EntersMap: "+identify(EntersMap));
	foreach(key,val in EntersMap){
	    //write("key: "+identify(key));
	    //write("key: "+typeof(key));
	    //write("val: "+identify(val));
	    //write("val: "+typeof(val));
	    if(arrayp(key) && member_array(dir,key) != -1) 
		key_arr = key;
	    true(); 
	}
	map_delete(EntersMap,key_arr);
	//tc("EntersMap: "+identify(EntersMap));
	map_str = "SetEnters( ([ \n";
	foreach( key, val in EntersMap){
	    //if(!functionp(PointlessMap[key]["pre"])) 
	    map_str += "\""+key[0]+"\" : \""+val+"\",\n";
	}
	map_str += "]) );";
	//write("map_str: "+map_str);

	unguarded( (: globalstr2 = read_file(globalstr) :) );
	contents = globalstr2;

	new_file = remove_matching_line(contents, "SetEnters", 1); 
	new_file = this_object()->eventAppend(new_file,({"AddItem","SetItems"}),"\n"+map_str+"\n");
	new_file = remove_matching_line(new_file,"SetObviousEnters");
	new_file = remove_matching_line(new_file,"//extras");
	new_file = remove_matching_line(new_file,"AddEnter(", 1);
	//write("new_file: "+new_file);
	globalstr2 = new_file;
	unguarded( (: write_file(globaltmp,globalstr2,1) :) );
	this_object()->eventGeneralStuff(tmpfile);
	globalstr = tmpfile;
	globalstr2 = filename;
	unguarded( (: cp(globalstr, globalstr2) :) );
	players = get_livings(environment(this_player()),1);
	load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
	players->eventMove(load_object(filename));
	//rm(tmpfile);
	write("With a puff of smoke, an enter vanishes!");
	return 1;
    }


    if(member_array(dir,load_object(filename)->GetEnters()) != -1){
	globalstr = filename;
	new_file = read_file(filename);
	file_arr = explode(read_file(filename),"\n");
	foreach(string linea in file_arr){
	    if(strsrch(linea,"AddEnter") != -1 && strsrch(linea,dir) != -1) {
		search_str = linea;
	    }
	}
	globalstr = tmpfile;
	globalstr2 = filename;
	globaltmp = remove_matching_line(new_file, search_str);
	unguarded( (: write_file(globalstr,globaltmp,1) :) );
	unguarded( (: cp(globalstr, globalstr2) :) );
	players = get_livings(environment(this_player()),1);
	load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
	eventProcessEnters(filename);
	players->eventMove(load_object(filename));
	rm(tmpfile);
	write("With a puff of smoke, an enter vanishes!");
	return 1;
    }

    write("This room's SetEnters does not contain that direction.");
    return 1;
}

varargs mixed eventProcessEnters(string filename, string dir, string location, object room) {
    string tmpenter, search_str, map_str, key, val, new_file;
    string *id_array;
    object *players, *dummies;
    mapping PointlessMap;

    mapping EntersMap = load_object(filename)->GetEnterMap();
    id_array = ({});
    dummies = load_object(filename)->GetDummyItems();
    if(sizeof(dummies)) foreach(object dumdum in dummies){
	id_array += dumdum->GetId();
    }

    if(file_exists(filename) && !check_privs(this_player(),filename)){
	write("You do not appear to have access to this file. Modification aborted.");
	return 1;
    }

    PointlessMap =  load_object(filename)->GetEnterMap();
    //tc("PointlessMap: "+identify(PointlessMap));
    //tc("EntersMap: "+identify(EntersMap));
    map_str = "SetEnters( ([\n";
    globalstr = filename;

    unguarded( (: this_object()->eventReadMapping(globalstr,({"SetEnters"}), 1) :) );
    new_file = remove_matching_line(unguarded( (: read_file(globalstr) :) ),"SetObviousExits");
    new_file = remove_matching_line(new_file,"//extras");
    new_file = remove_matching_line(new_file, "AddEnter(", 1, ":)");

    //foreach( key, val in EntersMap){
    //	if(!functionp(PointlessMap[key]["pre"])
    //           &&!functionp(PointlessMap[key]["post"]) 
    //      && present(key,load_object(filename)) )
    //                map_str += "\""+key+"\" : \""+val+"\",\n";
    //}

    foreach( key, val in EntersMap){
	//if(present(key[0],load_object(filename)) ) map_str += "\""+key[0]+"\" : \""+val+"\",\n";
	map_str += "\""+key[0]+"\" : \""+val+"\",\n";
    }
    //tc("dir: "+dir);
    //write("location: "+location);
    //tc("id_array: "+identify(id_array));

    if(dir && location && member_array(dir,id_array) != -1){
	map_str += "\""+dir+"\" : \""+location+"\",\n";
    }
    map_str += "]) );";

    globalstr = filename; 
    globaltmp = generate_tmp(load_object(filename));

    globalstr2 = this_object()->eventAppend(new_file,({"AddItem","SetItems","SetExits","AddExit"}),"\n"+map_str+"\n");
    unguarded( (: write_file(globaltmp,globalstr2,1) :) );
    unguarded( (: cp(globaltmp,globalstr) :) );
    load_object("/secure/cmds/creators/update")->cmd("-a "+filename);
    rm(globaltmp);
    return 1;
}
