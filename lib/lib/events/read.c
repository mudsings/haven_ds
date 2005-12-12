#include <function.h>

static private mixed Read    = 0;
static private mapping Reads = ([]);

// abstract methods
string GetShort();
// end abstract methods

string GetRead(string str) {
    if( !str || str == "default" ) {
	return Read;
    }
    else {
	return Reads[str];
    }
}

string array GetReads() {
    return keys(Reads);
}

mapping RemoveRead(string item) {
    if( !item || item == "default" ) {
	Read = 0;
    }
    else {
	map_delete(Reads, item);
    }
    return Reads;
}

varargs mixed SetRead(mixed arg1, mixed desc) {
    if( mapp(arg1) ) {
	Reads = expand_keys(arg1);
	if( Reads["default"] ) {
	    Read = Reads["default"];
	    map_delete(Reads, "default");
	}
    }
    if( !desc ) {
	Read = arg1;
	return Read;
    }
    if( !arg1 || arg1 == "default" ) {
	Read = desc;
	return Read;
    }
    if( arrayp(arg1) ) {
	foreach(string element in arg1) {
	    Reads[element] = desc;
	}
    }
    else {
	Reads[arg1] = desc;
    }
    return Reads;
}
varargs mixed SetDefaultRead(mixed arg1, mixed desc) {
    if( mapp(arg1) ) {
	return 0;
    }
    if( !desc ) {
	Read = arg1;
	return Read;
    }
    if( !arg1 || arg1 == "default" ) {
	Read = desc;
	return Read;
    }
}



varargs mixed eventRead(object who, string str) {
    mixed val = GetRead(str);

    if( arrayp(val) ) {
	val = val[query_night()];
    }
    if( functionp(val) ) {
	if( functionp(val) & FP_OWNER_DESTED ) {
	    who->eventPrint("There was a problem with the read.");
	    return 1;
	}
	//The funtion being evaluated, GetRead, only takes one arg.
	//return evaluate(val, who, str);
	return evaluate(val, str);
    }
    environment(who)->eventPrint(who->GetName() + " reads " + GetShort() + ".",
      who);
    if( !val ) {
	who->eventPrint("There is nothing to read.");
	return 1;
    }
    who->eventPrint(val);
    return 1;
}

mixed direct_read_obj() {
    if( !Read ) {
	return 0;
    }
    else {
	if( environment() != this_player()  && environment(this_player()) !=
	  environment()) {
	    return "#You don't have that!";
	}
	else return 1;

    }
}

mixed direct_read_str_word_obj(string str) {
    str = remove_article(lower_case(str));
    if( !Reads[str] ) {
	return 0;
    }
    else {
	if( environment() != this_player()  && environment(this_player()) !=
	  environment()) {
	    return "#You don't have that!";
	}
	else return 1;

    }
}

