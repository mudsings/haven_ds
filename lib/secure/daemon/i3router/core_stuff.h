#include <network.h>
#include <daemons.h>
#include <save.h>
#ifndef IMC2_SERVER_ENABLED
#define IMC2_SERVER_ENABLED 0
#endif

int heart_count = 0;

string router_port;
static mixed alternates = ({});
static int allow_multi_routers = 0;

static void create(){ 
    object rsock = find_object(RSOCKET_D);
    object ssock = find_object(SSOCKET_D);
    object sserver = find_object(IMC2_SERVER_D);

    if(irn_enabled && allow_multi_routers){
        foreach(string key, mixed val in routers){
            mixed tmp = ({ ({ key, val["ip"] + " " + val["port"] }) });
            if(key != "*dalet"){
                alternates += tmp;
            }
            alternates = sort_array(alternates, 1);
            tc("alternates: "+identify(alternates)); 
        }
    }
    if(!rsock){
        server_log("Strange. No rsocket.");
        catch( rsock = load_object(RSOCKET_D) );
        rsock->irn_clear();
    }
    if(!ssock && IMC2_SERVER_ENABLED){
        server_log("Strange. No ssocket.");
        catch( ssock = load_object(SSOCKET_D) );
    }
    if(!sserver && IMC2_SERVER_ENABLED){
        server_log("Strange. No sserver.");
        catch( sserver = load_object(IMC2_SERVER_D) );
    }
    if(!rsock && (!ssock || !sserver)) return;
    //tc("rsock: "+identify(rsock));
    //tc("ssock: "+identify(ssock));
    //tc("sserver: "+identify(sserver));
    SetNoClean(1);
    connected_muds = ([]);
    unguarded( (: restore_object, SAVE_ROUTER, 1 :) );
    if(!mudinfo) mudinfo = ([]);
    if(!mudinfo_updates) mudinfo_updates = ([]);
    if(!mudinfo_update_counter) mudinfo_update_counter = 1;
    if(!listening) listening = ([]);
    if(!channels) channels = ([]);
    if(!channel_updates) channel_updates = ([]);
    if(!channel_update_counter) channel_update_counter = 1;
    if(!router_name) router_name = "*yatmim";
    if(!router_port) router_port = "9000";
    if(!router_ip) router_ip = "149.152.218.102";
    if(sizeof(alternates)) router_list = alternates;
    else {
#if 0
        if(mud_name() == "Frontiers" || mud_name() == "*yatmim")
            router_list = ({ ({"*yatmim", "149.152.218.102 23"}) });
        if(mud_name() == "Dead Souls Aleph")
            router_list = ({ ({"*i6", "149.152.218.102 25"}) });
#endif
    }
    server_log("Created when mud uptime = " + time_elapsed(uptime()) + "\n");
    server_log("rsocket uptime = " + time_elapsed(time()-RSOCKET_D->GetInceptDate()) + "\n");
    call_out("setup", 1);
    call_out("LocalHostedChans", 15);
    this_object()->purge_crud();
    this_object()->check_discs();
    this_object()->build_ok_ips();
    set_heart_beat(10);
    if(file_exists(ROUTER_BLACKLIST)){
        blacklisted_muds += explode(read_file(ROUTER_BLACKLIST),"\n");
        blacklisted_muds = singular_array(blacklisted_muds);
    }
    this_object()->check_blacklist();
    trr("Current info:\n"+this_object()->get_info(1));
}

void heart_beat(){
    heart_count++;
    this_object()->send_pings();
    this_object()->irn_checkstat();
    this_object()->check_graylist();
    if(!(heart_count % 60)) {
        this_object()->check_discs();
        save_object(SAVE_ROUTER);
    }
    if(heart_count > 3600){
        heart_count = 0;
        this_object()->clean_ghosts();
        this_object()->clean_chans();
        this_object()->clear_discs();
        this_object()->check_blacklist();
    }
}

static void setup(){
    if( file_size( SAVE_ROUTER __SAVE_EXTENSION__ ) > 0 )
        unguarded( (: restore_object, SAVE_ROUTER, 1 :) );
    call_out("SetList",1);
}

int query_prevent_shadow(object ob){ return true(ob); }
