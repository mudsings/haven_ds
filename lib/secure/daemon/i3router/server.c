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
#define DEB_OUT_LOG 0	// //logging stuff
#define DEB_INVALID 3	// //trr-Invalid
#define DEB_OTHER 0	// //trr-Other
#define DEBUGGER_GUY "cratylus"	// Name of who to display //trrging info to.
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
static int max_age = 604800;
//static int max_age = 86400;
mapping Blacklist = ([]);

// Prototypes
static mapping muds_on_this_fd(int fd);
static mapping muds_not_on_this_fd(int fd);
void write_data(int fd, mixed data);
static void close_connection(int fd);
static void broadcast_data(mapping targets, mixed data);

// Ones with their own files...
string clean_fd(string fd);
static void broadcast_chanlist(string channame);
void broadcast_mudlist(string mudname);
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
#include "./server_log.h"
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
    //trr("connection close request for fd "+fd+", stack: "+get_stack(),"red");
    RSOCKET_D->close_connection(fd);
}

#if 1
varargs void write_data(int fd, mixed data, int override){
    mixed *sstat = socket_status(fd);
    string targetmud;
#if DEB_OUT_LOG
    write_file("/secure/log/tmp.txt","Trying to send at "+ctime(time()));
    write_file("/secure/log/tmp.txt"," packet size:"+sizeof(data));
    write_file("/secure/log/tmp.txt"," "+identify(data)+"\n---\n\n");
#endif
    if(data[0] == "irn-startup-req"){
        trr("%^B_YELLOW%^ATTENTION: IRN startup about to be sent to fd"+fd+":%^RESET%^ "+ identify(data),"red");
    }
    targetmud = this_object()->query_connected_fds()[fd];
    if(!sstat || sstat[1] != "DATA_XFER") return;
    if(!fd || (!data[4] && targetmud) ||  member_array(fd, keys(irn_sockets)) != -1 ||
      (targetmud && targetmud == data[4]) ){  
        RSOCKET_D->write_data(fd, data);
    }
    else  {
    }
}
#endif


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
    foreach(mixed element in socket_names()){
        if(intp(element[0]) && element[0] != -1 && !grepp(element[3],"*") &&
          last_string_element(element[3],".") == router_port &&
          member_array(element[0],
            keys(this_object()->query_irn_sockets())) == -1) {
            ret += ({ element[0] });
        }
    }
    return ret;
}

mixed get_info(int auto) {
    mixed *socky = sort_array(values(connected_muds), 1);
    mixed *muddy = sort_array(keys(connected_muds), 1);
    string socks = implode(socky, " ");
    string muddies = implode(muddy, ", ");
    int socknum = sizeof(socky);
    string ret = "";
    validate();

    socks += "\nTotal number of connected muds: "+socknum+"\n";
    ret = "router_name: "+router_name+
    "\nrouter_ip: "+router_ip+
    "\nrouter_port: "+router_port+
    "\nrouter_list"+identify(router_list)+
    "\nchannel_update_counter: "+ channel_update_counter+
    ((sizeof(channels)) ? "\nchannels:"+implode(sort_array(keys(channels),1),", ") : "")+
    "\nmudinfo_update_counter: "+ mudinfo_update_counter+
    "\nsockets: "+socks+
    "\nmuds: "+muddies+
    "\nRouter socket daemon uptime: "+
    time_elapsed(time()-RSOCKET_D->GetInceptDate())+
    ", up since "+ctime(RSOCKET_D->GetInceptDate())+
    "\n"+Report();

    if(auto) return ret;
    write(ret);
    return 1;
}

void clear(){ 
    string mudname; 
    validate();
    server_log("%^RED%^Clearing all mud data!"); 
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
    server_log(" setting router name to: "+str); 
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
    server_log("Setting router IP to: "+str);
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
    server_log("Setting router port to: "+str);
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
    server_log("Setting router list to: "+identify(router_list));
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
    server_log("Setting router list to: "+identify(router_list));
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
    server_log(str+" has been BANNED");
    save_object(SAVE_ROUTER);
    return banned_muds;
}

string *RemoveBannedMud(string str){
    validate();
    banned_muds -= ({ str });
    server_log(str+" has been unbanned.");
    save_object(SAVE_ROUTER);
    return banned_muds;
}

string *GetBlacklistedMuds(){
    validate();
    return blacklisted_muds;
}

varargs string *AddBlacklistedMud(string str, int perma){
    validate();
    if(!Blacklist) Blacklist = ([]);
    if(Blacklist[str]) return blacklisted_muds;
    if(perma) perma = 0;
    else perma = time();
    Blacklist[str] = perma;
    blacklisted_muds += ({ str });
    server_log(str+" has been BLACKLISTED");
    save_object(SAVE_ROUTER);
    return blacklisted_muds;
}

string *RemoveBlacklistedMud(string str){
    validate();
    if(!Blacklist) Blacklist = ([]);
    if(Blacklist[str]) map_delete(Blacklist, str);
    blacklisted_muds -= ({ str });
    server_log(str+" has been unblacklisted.");
    save_object(SAVE_ROUTER);
    return blacklisted_muds;
}

mixed GetRemoteIP(int fd){
    mixed conn = socket_status(fd);
    string ret;
    if(!conn || !sizeof(conn)) return 0;
    if(!conn[4] || conn[4] == "*.*") return 0;
    conn = explode(conn[4],".");
    ret = implode(conn[0..3],".");
    return ret;
}  

void check_blacklist(){
    int timenow = time();
    string *bms;
    trr("Checking old blacklists");
    if(!Blacklist) Blacklist = ([]);
    blacklisted_muds = distinct_array(blacklisted_muds);
    bms = keys(Blacklist);

    foreach(string element in blacklisted_muds){
        if(member_array(element, bms) == -1)
            Blacklist[element] = 0;
    }

    foreach(string key, int val in Blacklist){
        if((timenow - val) == timenow) continue;
        if((timenow - val) > max_age){
            RemoveBlacklistedMud(key);
            trr("%^B_BLACK%^Removed "+key+" from blacklist.","white");
        }
    }
}

void check_discs(){
    int *fds = values(connected_muds);
    int i = 1;

    foreach(string mudname in keys(mudinfo)){
        if(!connected_muds[mudname] && mudinfo[mudname]["router"]){
            if(mudinfo[mudname]["router"] != my_name &&
              member_array(mudinfo[mudname]["router"],keys(irn_connections)) == -1){
                if(!mudinfo[mudname]["disconnect_time"]){
                    trr("killing "+mudname);
                    mudinfo[mudname]["disconnect_time"] = 100;
                    if(mudinfo[mudname]["connect_time"])
                        mudinfo[mudname]["connect_time"] = 0;
                    schedule_broadcast(mudname);
                }
            }
        }
    }

    if(sizeof(fds))
        foreach(mixed element in sort_array(fds,1)){
        string lost_mud;
        if(!intp(element)) continue;
        if(!socket_status(element) ||
          socket_status(element)[1] == "CLOSED" || !GetRemoteIP(element) ||
          GetRemoteIP(element) != mudinfo[query_connected_fds()[element]]["ip"]){
            foreach(string key, mixed val in mudinfo){
                if(!connected_muds[key] && mudinfo[key]["router"]){
                    if(mudinfo[key]["router"] == my_name){
                        server_log("Cleaning connection info from "+key);
                        if(!mudinfo[key]["disconnect_time"])
                            mudinfo[key]["disconnect_time"] = time();
                        if(mudinfo[key]["connect_time"]) 
                            mudinfo[key]["connect_time"] = 0;
                    }
                    else {
                        server_log("Cleaning connection info from "+key);
                        if(!mudinfo[key]["disconnect_time"])
                            mudinfo[key]["disconnect_time"] = 0;
                        if(mudinfo[key]["connect_time"]) 
                            mudinfo[key]["connect_time"] = 0;
                    }
                }
            }

            foreach(string key, int val in connected_muds){
                if(val == element){
                    trr("REMOVING DISCONNECTED: "+key+" from "+val);
                    i++;
                    call_out( (: disconnect_mud :), i, key);
                }
            }
        }
    }
}

void clean_ghosts(){
    int tmp,i;
    object rsockd = find_object(RSOCKET_D);
    mixed array incoming = socket_status();
    mixed *legit_socks = keys(this_object()->query_socks());
    legit_socks += keys(this_object()->query_irn_sockets());

    if(!rsockd) return;

    tmp =sizeof(socket_status())-1;

    for(i=0;i < tmp;i++){ 
        if(!incoming[i][5] || incoming[i][5] != rsockd) continue;
        if(member_array(i, legit_socks) == -1 && incoming[i][1] == "DATA_XFER"){ 
            this_object()->close_connection(i);
        }
    }
}

void clean_chans(){
    string *cleaned = ({});
    foreach(mixed key, mixed val in listening){
        if(arrayp(val)){
            foreach(string listening_mud in val){
                if(!mudinfo[listening_mud]){
                    listening[key] -= ({ listening_mud });
                    cleaned += ({ listening_mud });
                }
            }
        }
        else trr("huh? not an array?");
    }
    foreach(mixed key, mixed val in channels){
        mixed *tmp_chan = ({});
        if(sizeof(val) == 3){
            tmp_chan = ({ val[0], val[1], distinct_array(val[2]) });
            channels[key] = tmp_chan;
        }
    }
    cleaned = distinct_array(cleaned);
    save_object(SAVE_ROUTER);
    trr("channel cleanup: cleaned from listening: "+implode(cleaned,"\n"));
}

void clear_discs(){ 
    string mudname; 
    int i = 1;

    validate();

    trr("%^B_BLACK%^Discarding excessively old disconnects.","white");
    trr("%^B_BLACK%^Max age is "+time_elapsed(max_age)+".","white");
    foreach(mudname in keys(mudinfo)) {
        int deadsince = time() - mudinfo[mudname]["disconnect_time"];

        if(!connected_muds[mudname] && mudinfo[mudname]["router"]){
            if(mudinfo[mudname]["router"] != my_name && 
              member_array(mudinfo[mudname]["router"],keys(irn_connections)) == -1){
                if(!mudinfo[mudname]["disconnect_time"])
                    mudinfo[mudname]["disconnect_time"] = 0;
                if(mudinfo[mudname]["connect_time"])
                    mudinfo[mudname]["connect_time"] = 0;
            }
        }

        if(!mudinfo[mudname]["disconnect_time"]) continue;
        trr("%^B_BLACK%^"+mudname+": "+ctime((mudinfo[mudname]["disconnect_time"]) || time()),"white");
        trr("%^B_BLACK%^dead age: "+time_elapsed(deadsince),"white");
        if(mudinfo[mudname]["disconnect_time"] && deadsince > max_age ){
            trr(mudname+": "+time_elapsed(deadsince),"red");
            server_log("I want to remove "+mudname+". Its disconnect time is "+ctime(query_mud(mudname)["disconnect_time"]));
            server_log("Which was "+time_elapsed(time() - query_mud(mudname)["disconnect_time"])+" ago.");
            i = i+1;
            remove_mud(mudname,1);
        }

        if(mudinfo[mudname] && mudinfo[mudname]["disconnect_time"] > 0 &&
          mudinfo[mudname]["connect_time"] > 0){
            i = 1;
            server_log("I want to remove "+mudname+". It is in a paradox state.");
            if(member_array(mudname,keys(query_connected_muds())) != -1){
                trr("%^B_BLACK%^Its fd is: "+query_connected_muds()[mudname],"white");
            }
            else {
                trr("%^B_BLACK%^It is not listed as a connected mud.","white");
            }
            server_log("Removing disconnected mud: "+identify(mudname));
            i = i+2;
            remove_mud(mudname,1);
        }
    }
}

int eventDestruct(){
    validate();
    save_object(SAVE_ROUTER);
    server_log("I am being destructed by: \n"+get_stack()+
      "\n"+identify(previous_object(-1)));
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

varargs void ReceiveList(mixed data, string type, string who){
    string *cmuds = keys(connected_muds);
    //trr("ReceiveList("+identify(data)+", "+identify(type)+", "+identify(who)+")");
    if(!type || !sizeof(type)) type = "mudlist";
    if(!mapp(data)){
        return;
    }
    if(type == "mudlist"){
        foreach(mixed key, mixed val in data){
            if(member_array(key,cmuds) != -1) continue;
            if(mudinfo[key] && (!val || intp(val))){
                trr("ROUTER_D: deleting "+key);
                remove_mud(key,1);
                continue;
            }
            if(!mapp(val)){
                return;
            }
            mudinfo_update_counter++;
            if(!connected_muds[key]){
                trr("%^B_GREEN%^%^BLACK%^accepting "+key+
                  " update from "+(mudinfo[key] ? mudinfo[key]["router"] : who ));
                mudinfo_updates[key] = mudinfo_update_counter;
                mudinfo[key]=val;
                schedule_broadcast(key, 1);
            }
            else {
                trr("%^RESET%^Looks like I'm %^B_RED%^%^BLACK%^REJECTING%^RESET%^ "+key+"."
                  +mudinfo[key]["router"]+" update from "+who);
            }
        }
    }
    else if(type == "chanlist"){
        if(data["listening"] && sizeof(data["listening"]) && mapp(data["listening"])){
            foreach(mixed key, mixed val in data["listening"]){
                trr("listening update: "+key+" is "+identify(val)+" val is a "+typeof(val),"yellow");
            }
        }
        if(data["channels"] && sizeof(data["channels"]) && mapp(data["channels"])){
            foreach(mixed key, mixed val in data["channels"]){
                string ownermud = data["channels"][key][1];
                if(!key || !val || !stringp(key)){
                    map_delete(channels, key);
                    map_delete(listening, key);
                    continue;
                }
                if(member_array(ownermud, cmuds) != -1) continue;
                if(val == -1) map_delete(channels, key);
                if(val == -1) map_delete(listening, key);
                else {
                    val[2] = distinct_array(val[2]);
                    channels[key] = val;
                }
                broadcast_chanlist(key);
                trr("chan update: "+key+" is "+identify(val)+" val is a "+typeof(val),"green");
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

varargs int purge_ip(string ip, int rude, mixed *sock_array){
    validate();
    if(!sock_array || !sizeof(sock_array)) sock_array = socket_names();
    foreach(mixed element in sock_array){
        int fd = element[0];
        if(last_string_element(element[3],".") != router_port) continue;
        if(member_array(fd, keys(irn_sockets)) != -1) continue;
        if(ip == "*") continue;
        if(clean_fd(socket_address(fd)) == ip){
            if(query_connected_fds()[fd]){
                if(rude){
                    server_log("router: fd to be rudely purged: "+fd+", "+query_connected_fds()[fd]);
                    disconnect_mud(query_connected_fds()[fd]);
                }
            }
        }
    }
    return 1;
}

varargs int purge_ips(int rude){
    mixed *sockies = socket_names();
    foreach(mixed element in sockies){
        string ip_address = element[3];
        if(last_string_element(ip_address,".") == router_port ){
            ip_address = replace_string(element[4],"."+last_string_element(element[4],"."),"");
            purge_ip(ip_address,(rude || 0), sockies);
        }
    }
    return 1;
}
