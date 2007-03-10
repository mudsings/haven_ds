// I3 server.
// This file written mostly by Tim Johnson (Tim@TimMUD)
// Started by Tim on May 7, 2003.
// http://cie.imaginary.com/protocols/intermud3.html#errors
#include <lib.h>
#include <commands.h>
#include <daemons.h>
#include <socket.h>
#define MAXMUDS 32	// Max number of muds allowed from one IP
#define ROUTER_BLACKLIST "/secure/daemon/i3router/blacklist.cfg" //bad IP's
#define DEB_IN  1	// //trr-Incoming
#define DEB_OUT 2	// //trr-Outgoing
#define DEB_INVALID 3	// //trr-Invalid
#define DEB_OTHER 0	// //trr-Other
#define DEBUGGER_GUY "cratylus"	// Name of who to display //trrging info to.
#undef DEBUGGER_GUY
#define DEBUGGER_GUY "MXLPLX"
#define MAXIMUM_RETRIES 20
// SEND_WHOLE_MUDLIST makes it act like the official I3 server instead of like the I3 specs
#define SEND_WHOLE_MUDLIST
// SEND_WHOLE_CHANLIST makes it act like the official I3 server instead of like the I3 specs
#define SEND_WHOLE_CHANLIST
inherit LIB_DAEMON;

object cmd = load_object(CMD_ROUTER);
object rsocket = find_object(RSOCKET_D);

static void validate(){
    if( previous_object() != cmd && previous_object() != rsocket &&
      previous_object() != this_object() &&
      !((int)master()->valid_apply(({ "ASSIST" }))) ){
	trr("SECURITY ALERT: validation failure in ROUTER_D.","red");
	error("Illegal attempt to access router daemon: "+get_stack()+
	  " "+identify(previous_object(-1)));
    }
}

// Saved variables...
mapping listening;
// list of muds listening to each channel
// (key=chan name, value=mud array)
mapping connected_muds;
// muds that have successfully done a startup
// (key=mudname, value=fd)
string router_name; // Name of the router.
string router_ip;
string *router_list = ({}); // Ordered list of routers to use.
mapping mudinfo = ([]); // Info about all the muds which the router knows about.
mapping channels; // Info about all the channels the router handles.
mapping channel_updates; // Tells when a channel was last changed.
int channel_update_counter; // Counter for the most recent change.
// Why is this not a part of the channels mapping?
// Because I need to remember that some channels got deleted.
mapping mudinfo_updates; // Like channel_updates except for muds.
int mudinfo_update_counter; // Similar to channel_update_counter

// Prototypes
static mapping muds_on_this_fd(int fd);
static mapping muds_not_on_this_fd(int fd);
void write_data(int fd, mixed data);
static void close_connection(int fd);
static void broadcast_data(mapping targets, mixed data);

// Ones with their own files...
string clean_fd(string fd);
static void broadcast_chanlist(string channame);
static void broadcast_mudlist(string mudname);
static varargs void Debug(string str, int level);
static void process_channel(int fd, mixed *info);
static void process_startup_req(int protocol, mixed info, int fd);
static void read_callback(int fd, mixed info);
static void remove_mud(string mudname, int forced);
static void send_chanlist_reply(string mudname, int old_chanid);
static void send_mudlist(string mudname);
static void send_mudlist_updates(string updating_mudname, int old_mudlist_id);
static void send_startup_reply(string mudname);
static void send_error(string mud, string user, string errcode, string errmsg, mixed *info);
void send_full_mudlist(string mud);
// core_stuff.h...
static void create();
static void setup();
void remove();
// funcs.h...
static mapping muds_on_this_fd(int fd);
int value_equals(string a,int b, int c);
static mapping muds_not_on_this_fd(int fd);
int value_not_equals(string a,int b, int c);
// socket_stuff.h
varargs string *SetList();

// Code for all the stuff in the prototypes...
#include "./irn.h"
#include "./clean_fd.h"
#include "./broadcast_chanlist.h"
#include "./broadcast_mudlist.h"
#include "./debug.h"
#include "./process_channel.h"
#include "./process_startup_req.h"
#include "./read_callback.h"
#include "./remove_mud.h"
#include "./send_chanlist_reply.h"
#include "./send_mudlist_updates.h"
#include "./send_startup_reply.h"
#include "./send_error.h"

#include "./core_stuff.h"
#include "./funcs.h"
#include "./hosted_channels.h"
#include "./send_full_mudlist.h"

static void close_connection(int fd){
    RSOCKET_D->close_connection(fd);
}

void write_data(int fd, mixed data){
    RSOCKET_D->write_data(fd, data);
}

static void broadcast_data(mapping targets, mixed data){
    RSOCKET_D->broadcast_data(targets, data);
}

// debugging stuff...
mapping query_mudinfo(){ validate(); return copy(mudinfo); }
mapping query_mud(string str){ validate(); return copy(mudinfo[str]); }
mapping query_connected_muds(){ validate(); return copy(connected_muds); }
mapping query_socks(){ validate(); return RSOCKET_D->query_socks(); }
mapping query_chaninfo(){ return ([ "listening" : listening, "channels" : channels ]); }

mapping query_connected_fds(){
    mapping RetMap = ([]);
    validate();
    foreach(mixed key, mixed val in connected_muds){
	RetMap[val] = key;
    }
    return copy(RetMap);
}

int *open_socks(){
    int *ret = ({});
    validate();
    foreach(mixed element in socket_status()){
	if(intp(element[0]) && element[0] != -1 && !grepp(element[3],"*") &&
	  last_string_element(element[3],".") == router_port) {
	    ret += ({ element[0] });
	}
    }
    return ret;
}

void get_info() {
    mixed *socky = open_socks();
    string socks = implode(socky, " ");
    int socknum = sizeof(socky);
    validate();

    socks += "\nTotal number of connected muds: "+socknum+"\n";
    write_file ("/secure/tmp/info.txt",
      "router_name: "+router_name+
      "\nrouter_ip: "+router_ip+
      "\nrouter_port: "+router_port+
      "\nrouter_list"+identify(router_list)+
      "\nchannel_update_counter: "+ channel_update_counter+
      "\nchannels:"+identify(channels)+
      "\nchannel_updates:"+identify(channel_updates)+
      //"\nlistening:"+identify(listening)+
      //"\nmudinfo:"+identify(mudinfo)+
      "\nmudinfo_update_counter: "+ mudinfo_update_counter+
      "\nmudinfo_updates:"+identify(mudinfo_updates)+
      "\nconnected:"+identify(connected_muds)+"\n");
    write("router_name: "+router_name+
      "\nrouter_ip: "+router_ip+
      "\nrouter_port: "+router_port+
      "\nrouter_list"+identify(router_list)+
      "\nchannel_update_counter: "+ channel_update_counter+
      ((sizeof(channels)) ? "\nchannels:"+implode(keys(channels),", ") : "")+
      "\nmudinfo_update_counter: "+ mudinfo_update_counter+
      "\nsockets: "+socks+
      "\n"+Report()
    );
}

void clear(){ 
    string mudname; 
    validate();
    log_file("router/server_log",timestamp()+" Clearing all mud data.\n"); 
    foreach(mudname in keys(mudinfo)) remove_mud(mudname,1); 
    save_object(SAVE_ROUTER);    
}

string GetRouterName(){
    validate();
    return router_name;
}

string SetRouterName(string str){
    validate();
    if(first(str,1) != "*") str = "*"+str;
    router_name = str;
    log_file("router/server_log",timestamp()+" setting router name to: "+str+"\n"); 
    SetList();
    return router_name;
}

string GetRouterIP(){
    validate();
    return router_ip;
}

string SetRouterIP(string str){
    validate();
    router_ip = str;
    log_file("router/server_log",timestamp()+" setting router IP to: "+str+"\n");
    SetList();
    return router_ip;
}

string GetRouterPort(){
    validate();
    return router_port;
}

string SetRouterPort(string str){
    validate();
    router_port = str;
    log_file("router/server_log",timestamp()+" setting router port to: "+str+"\n");
    SetList();
    return router_port;
}

string *GetRouterList(){
    validate();
    return router_list;
}

varargs string *SetList(){
    string tmp;
    string tmp_port = router_port;
    string tmp_ip = router_ip;
    validate();
    if(!strsrch(router_name,"*")) tmp = router_name;
    else tmp = "*"+router_name;
    if(lower_case(mud_name()) == "frontiers"){
	tmp_port = "23";
	tmp_ip = "149.152.218.102";
	tmp = "*yatmim";
    }
    router_list = ({ ({ tmp, tmp_ip+" "+tmp_port }) });
    save_object(SAVE_ROUTER);
    log_file("router/server_log",timestamp()+" setting router list to: "+identify(router_list)+"\n");
    save_object(SAVE_ROUTER);
    return router_list;
}

varargs string *SetRouterList(string *str){
    string tmp;
    string tmp_port = router_port;
    string tmp_ip = router_ip;
    validate();
    return router_list;
    if(!strsrch(router_name,"*")) tmp = router_name;
    else tmp = "*"+router_name;
    if(!str || !sizeof(str)){
	if(lower_case(mud_name()) == "frontiers"){
	    tmp_port = "23";
	    tmp_ip = "149.152.218.102";
	    tmp = "*yatmim";
	}
	router_list = ({ ({ tmp, tmp_ip+" "+tmp_port }) });
	save_object(SAVE_ROUTER);
	return router_list;
    }
    router_list = ({ str });
    log_file("router/server_log",timestamp()+" setting router list to: "+identify(router_list)+"\n");
    save_object(SAVE_ROUTER);
    return router_list;
}

mapping GetConnectedMuds(){
    validate();
    return copy(connected_muds);
}

string *GetBannedMuds(){
    validate();
    return banned_muds;
}

string *AddBannedMud(string str){
    validate();
    banned_muds += ({ str });
    log_file("router/server_log",timestamp()+" "+str+" has been BANNED\n");
    save_object(SAVE_ROUTER);
    return banned_muds;
}

string *RemoveBannedMud(string str){
    validate();
    banned_muds -= ({ str });
    log_file("router/server_log",timestamp()+" "+str+" has been unbanned.\n");
    save_object(SAVE_ROUTER);
    return banned_muds;
}

string *GetBlacklistedMuds(){
    validate();
    return blacklisted_muds;
}

string *AddBlacklistedMud(string str){
    validate();
    blacklisted_muds += ({ str });
    log_file("router/server_log",timestamp()+" "+str+" has been BLACKLISTED\n");
    save_object(SAVE_ROUTER);
    return blacklisted_muds;
}

string *RemoveBlacklistedMud(string str){
    validate();
    blacklisted_muds -= ({ str });
    log_file("router/server_log",timestamp()+" "+str+" has been unblacklisted.\n");
    save_object(SAVE_ROUTER);
    return blacklisted_muds;
}

void check_discs(){
    int *fds = values(connected_muds);
    fds += keys(irn_sockets);
    foreach(int element in sort_array(fds,1)){
	string lost_mud;
	if(!socket_status(element) ||
	  socket_status(element)[1] == "CLOSED"){
	    foreach(string key, mixed val in mudinfo){
		if(!connected_muds[key] && mudinfo[key]["router"] && mudinfo[key]["router"] == my_name){
		    disconnect_mud(key);
		}
	    }
	    foreach(string key, int val in connected_muds){
		if(val == element){
		    trr("REMOVING DISCONNECTED: "+key+" from "+val);
		    disconnect_mud(key);
		}
	    }
	    foreach(mixed key, mixed val in irn_sockets){
		map_delete(irn_sockets, key);
	    }
	}
    }
}

void clear_discs(){ 
    string mudname; 
    validate();
    foreach(mudname in keys(mudinfo)) {
	if(query_mud(mudname)["disconnect_time"] > 604800 ){
	    //trr("I want to remove "+mudname+". Its disconnect time is "+ctime(query_mud(mudname)["disconnect_time"]),"white");
	    //trr("Which was "+time_elapsed(time() - query_mud(mudname)["disconnect_time"])+" ago.","white");
	    if(member_array(mudname,keys(query_connected_muds())) != -1){
		//trr("Its fd is: "+query_connected_muds()[mudname],"white");
	    }
	    else {
		//trr("It is not listed as a connected mud.","white");
	    }
	    //trr("Removing disconnected mud: "+identify(mudname),"red");
	    remove_mud(mudname,1);
	}
	if(mudinfo[mudname] && mudinfo[mudname]["disconnect_time"] > 0 &&
	  mudinfo[mudname]["connect_time"] > 0){
	    //trr("I want to remove "+mudname+". It is in a paradox state.","white");
	    if(member_array(mudname,keys(query_connected_muds())) != -1){
		//trr("Its fd is: "+query_connected_muds()[mudname],"white");
	    }
	    else {
		//trr("It is not listed as a connected mud.","white");
	    }
	    //trr("Removing disconnected mud: "+identify(mudname),"red");
	    remove_mud(mudname,1);
	}
    }
}

int eventDestruct(){
    validate();
    save_object(SAVE_ROUTER);
    daemon::eventDestruct();
}

string query_fd_info(mixed foo){
    int num, i;
    string ret = "";    validate();
    if(stringp(foo)) if(sscanf(foo,"%d",num) != 1) return "foo";
    if(intp(foo)) num = foo;
    for(i=0;i<num;i++){
	mapping bar = query_connected_fds();
	ret += i+" "+socket_address(i)+" "+(bar[i]+"" || "")+"\n";
    }
    return ret;
}

int GetMaxRetries(){ return MAXIMUM_RETRIES; }

varargs void ReceiveList(mixed data, string type){
    //trr("ReceiveList: " +identify(keys(data)),"red");
    //trr("ReceiveList ("+type+"): " +identify(data),"red");
    //trr("Current muds: "+identify(keys(mudinfo)));
    if(!type || !sizeof(type)) type = "mudlist";
    if(!mapp(data)){
	//trr("router: ReceiveList: NOT A MAPPING: "+get_stack(),"red");
	return;
    }
    if(type == "mudlist"){
	foreach(mixed key, mixed val in data){
	    if(mudinfo[key] && intp(val)){
		trr("ROUTER_D: deleting "+key);
		mudinfo[key]["disconnect_time"] = time();
		mudinfo[key]["connect_time"] = 0;
		broadcast_mudlist(key, 1);
		map_delete(mudinfo, key);
		continue;
	    }
	    if(!mudinfo[key] || member_array(key,keys(connected_muds)) == -1){
		//trr("received remote list: "+key,"white");
		if(!mapp(val)){
		    //trr("VAL NOT A MAPPING. key: "+identify(key)+", val: "+identify(val),"blue");
		    return;
		}
		//trr("accepting "+key+" into mudinfo mapping.","cyan");
		mudinfo_update_counter++;
		mudinfo_updates[key] = mudinfo_update_counter;
		mudinfo[key]=val;
		broadcast_mudlist(key, 1);
	    }
	    else {
		//trr("router: REJECTING "+key+".","white");
	    }
	}
    }
    else if(type == "chanlist"){
	//trr("Current muds: "+identify(keys(mudinfo)));
	if(data["listening"] && sizeof(data["listening"]) && mapp(data["listening"])){
	    foreach(mixed key, mixed val in data["listening"]){
		listening[key] = val;
		//trr("listening update: "+key+" is "+identify(val));
	    }
	}
	if(data["channels"] && sizeof(data["channels"]) && mapp(data["channels"])){
	    foreach(mixed key, mixed val in data["channels"]){
		if(val == -1) map_delete(channels, key);
		if(val == -1) map_delete(listening, key);
		else {
		    val[2] = singular_array(val[2]);
		    channels[key] = val;
		}
		broadcast_chanlist(key);
		//trr("chan update: "+key+" is "+identify(val));
	    }
	}
    }
    save_object(SAVE_ROUTER);
}

int purge_crud(){
    foreach(mixed key, mixed val in mudinfo){
	if(!mapp(val)) map_delete(mudinfo, key);
    }
    save_object(SAVE_ROUTER);
    return sizeof(mudinfo);
}

varargs int purge_ip(string ip, int rude){
    mixed *sock_array = socket_status();
    validate();
    foreach(mixed element in sock_array){
	int fd = member_array(element, sock_array);
	if(last_string_element(socket_status(fd)[3],".") != router_port) continue;
	if(member_array(fd,keys(irn_sockets)) != -1) continue;
	if(clean_fd(socket_address(fd)) == ip){
	    if(query_connected_fds()[fd]){
		if(rude){
		    trr("router: fd to be purged: "+fd+", "+query_connected_fds()[fd]);
		    disconnect_mud(query_connected_fds()[fd]);
		}
	    }
	    else {
		trr("router: purging fd:"+fd+", status: "+identify(socket_status(fd)));
		close_connection(fd);
	    }
	}
    }
    return 1;
}

varargs int purge_ips(int rude){
    int i, quant = sizeof(socket_status());
    string ip_address;
    for(i = 0; i < quant; i++){
	ip_address = socket_status(i)[3];
	if(last_string_element(ip_address,".") == router_port ){
	    ip_address = replace_string(socket_status(i)[4],"."+last_string_element(socket_status(i)[4],"."),"");
	    purge_ip(ip_address,(rude || 0));
	}
    }
    return 1;
}



