/*    /daemon/services/who.c
 *    from the Dead Souls LPC Library
 *    handles the Intermud 3 who service
 *    created by Descartes of Borg 950623
 */

#define SERVICE_WHO

#include <daemons.h>
#include <rooms.h>
static mapping user_table = ([]);

int mini;

static string eventLookupUser(string str){
    if(!user_table) user_table = ([]);
    if(!user_table[str]) return str;
    else {
        string ret = user_table[str];
        //map_delete(user_table, str);
        return ret;
    }
}

void eventReceiveWhoReply(mixed *packet) {
    string list, *who, tmp;
    object ob;

    if( file_name(previous_object()) != INTERMUD_D ) return;
    if( !packet[5] || !(ob = find_player(convert_name(eventLookupUser(packet[5])))) ) return;
    list = "%^MAGENTA%^Remote who information from " + packet[2] + ":%^RESET%^\n";
    foreach(who in packet[6]){ 
        mixed wtf;
        if(intp(who[1])) wtf = to_int(who[1]);
        if(wtf < 6) tmp = "not";
        else tmp = time_elapsed(wtf);
        list +=  who[0] + " (" + tmp + " idle): " + who[2] +"\n";
    }
    ob->eventPrint(list);
    tn("eventReceiveWhoReply: "+identify(packet),"blue");
}

void eventReceiveWhoRequest(mixed *packet) {
    mixed *msg;
    string *real_users = ({});
    string ret = "";
    mini = 0;
    real_users = filter(users(), (: $1->GetKeyName() == last_string_element(base_name($1),"/") :));
    if( file_name(previous_object()) != INTERMUD_D ) return;
    if(sizeof(real_users) > 100) mini = 1;
    msg = map(filter(real_users, (: (environment($1) && !((int)$1->GetInvis()))  :)),
      (: ({ (string)$1->GetCapName(), query_idle($1),
          (mini ? "" : (string)$1->GetShort()) }) :));
    if(sizeof(identify(msg)) >= __LARGEST_PRINTABLE_STRING__){
        msg = ({({"Number of users: "+sizeof(msg), 0, "Too many results." })});
    }
    write_file("/tmp/thingy.txt",identify(msg),1);
    INTERMUD_D->eventWrite(({ "who-reply", 5, mud_name(), 0, packet[2],
        packet[3], msg }));
    foreach(string *entry in msg){
        ret += entry[0]+", ";
    }
    ret = truncate(ret,2);
    tn("eventReceiveWhoRequest: "+identify(packet),"blue");
    tell_room(ROOM_ARCH,"The Arch Room loudspeaker announces: \"%^BOLD%^CYAN%^"+capitalize(packet[3])+" at "+packet[2]+" has requested a list of users currently logged on. Replying with: %^BOLD%^YELLOW%^"+ret+".%^RESET%^\"");
    CHAT_D->eventSendChannel("SYSTEM","intermud","[" + capitalize(packet[3])+"@"+packet[2]+
      " requests the who list]",0);
}

varargs void eventSendWhoRequest(string mud) {
    string who, crypt_who;

    who = (string)this_player(1)->GetKeyName();

    if(this_player(1)->GetInvis()){
        foreach(string key, string val in user_table){
            if(!key || ! val) continue;
            if(val == who){
                crypt_who = key;
            }
        }
        if(!crypt_who){
            crypt_who = alpha_crypt(10); 
            user_table[crypt_who] = who;
        }
    }

    else crypt_who = who;

    if((mud) && sizeof(mud)) INTERMUD_D->eventWrite(({ "who-req", 5, mud_name(), crypt_who, mud, 0 }));
    else INTERMUD_D->eventWrite(({ "who-req", 5, mud_name(), crypt_who, 0, 0 }));
    tn("eventSendWhoRequest: "+identify( ({ "who-req", 5, mud_name(), who, mud, 0 })), "blue");
}

