#include <objects.h>
#include <save.h>
#include <lib.h>
#include <cfg.h>
#include <daemons.h>
#include <dirs.h>
#ifndef AUTO_ADVANCE
#define AUTO_ADVANCE 0
#endif
#ifndef REQUIRE_QUESTING
#define REQUIRE_QUESTING 1
#endif
#ifndef ENABLE_INSTANCES
#define ENABLE_INSTANCES 0
#endif

inherit LIB_DAEMON;

mapping PlayerDataMap = ([]);
string *PendingEncres = ({});
string *PendingDecres = ({});
string *players = ({});
string *creators = ({});
string *user_list = ({});
static object ob;
static string gplayer, SaveFile;
static int maxlevel;
static string LevelList = "";

string player_save_file;
static string namestr = "";

//Player vars
string InternalDesc, Short, CapName, ExternalDesc, CurrentWorkingDirectory, PreviousWorkingDirectory, Terminal;
string Password, Email, RealName, Rank, LoginSite, HostSite, Torso, DefaultLanguage, Town, Race;
string Gender, Class, WimpyCommand, InternalDesc, WebPage, Clan;
int ProperNoun, Opacity, Modify, Invisible, NoChanColors, Paused, MaxCommandHistSize, HistorySize;
int Age, WhereBlock, Brief, LoginTime, BirthTime, RescueBit, CarriedMass, MaxCarry, Position;
int  Undead, UndeadType, Mount, BodyMass, SaveRecurse, Retain, HealthPoints, MagicPoints, ExperiencePoints;
int QuestPoints, melee, Alcohol, Caffeine, Food, Drink, Poison, Sleeping, DeathEvents, Biter;
int Blind, CustomStats, Wimpy, Polyglot, Level, Morality, Dead, isPK, Deaths, TrainingPoints;
int TitleLength, CreatorAge, CreatorBirth;
object Agent;
float StaminaPoints;
mixed array Inventory, CommandHist, Screen,  Marriages, Saved, ExtraChannels, LightSensitivity;
mixed array Religion, Titles, Muffed, Quests; 
mapping Items, Properties, Messages, RestrictedChannels, Nicknames, Aliases, Xverbs, Blocked;
mapping News, Fingers, Limbs, MissingLimbs, Resistance, Stats, Languages, Skills, SkillModifiers;
mapping Currency, Bank, SpellBook; 
//end player vars

mapping Levels = ([]);
static mapping LevelTitles = ([
        1:"the utter novice",
        2:"the simple novice",
        3:"the beginner",
        4:"the adventurer",
        5:"the experienced adventurer",
        6:"the expert adventurer",
        7:"the great adventurer",
        8:"the master adventurer",
        9:"the Freeman",
        10:"the Citizen",
        11:"the Knight",
        12:"the Baron",
        13:"the Count",
        14:"the Earl",
        15:"the Marquis",
        16:"the Duke",
        17:"the Arch Duke",
        18:"the Praetor",
        19:"the Quaestor",
        20:"the Caesar"
]);

static mapping QuestLevels = ([
        10:5,
        12:12,
        14:21,
        16:32,
        18:45,
        20:60,
        22:77,
        24:96,
        26:117,
        28:140,
        30:165,
        32:192,
        34:221,
        36:252,
        38:285,
        40:450,
        ]);


void validate(){
    if(!(int)master()->valid_apply(({ "SECURE", "ASSIST", "LIB_CONNECT" })) &&
            base_name(previous_object()) != "/www/cgi/login"){
        string offender = identify(previous_object(-1));
        debug("PLAYERS_D SECURITY VIOLATION: "+offender+" ",get_stack(),"red");
        log_file("security", "\n"+timestamp()+" PLAYERS_D breach: "+offender+" "+get_stack());
        error("PLAYERS_D SECURITY VIOLATION: "+offender+" "+get_stack());
    }
}

// This function generates a table of required xp per level.
// The xp required for lower levels is high, approximately
// half-again the xp required for the previous level. As the
// player progresses, this ratio decreases.
mapping CompileLevelList(){
    int i=1;
    int seed=300;
    float mod;
    LevelList = "";
    Levels = ([ 0 : ([ "xp" : 0, "qp" : 0 ]) ]);
    Levels[1] = ([ "title" : "the utter novice ", "xp" : 0, "qp" : 0 ]);
    while(seed > 0){
        i++;
        mod = 50/i;
        if(mod > 9) mod = 9.0;
        if(mod < 1) mod = 0.5;
        if(i > 100) mod = 0.1;
        mod *= 0.1;
        seed = seed * (1+mod); 
        seed = ((seed/100) * 100);
        LevelList += "level: "+i+", ";
        if(seed > 0){
            Levels[i] = (["xp" : seed ]);
            if(REQUIRE_QUESTING){
                if(QuestLevels[i]){
                    Levels[i]["qp"] = QuestLevels[i];
                }
            }
            else Levels[i]["qp"] = 0;
            if(LevelTitles[i]){
                Levels[i]["title"] = LevelTitles[i];
            }
            maxlevel = i;
            LevelList += ( Levels[i]["title"] || "Untitled" );
            LevelList += ", exp: "+seed;
            LevelList += ( Levels[i]["qp"] ? ", qp: "+Levels[i]["qp"] : "");
            LevelList += "\n";
        }
        if(seed >= 2146000000) break;
    }
    return copy(Levels);
}

static void ScheduledPlayerAdd(string *plays){
    foreach(string play in plays){
        this_object()->AddPlayerInfo(play);
    }
}

string *CompileCreList(){
    string *cre_dirs = unguarded( (: get_dir(DIR_CRES+"/") :) );
    string *cres = ({});
    string *cache = ({});
    int i;
    creators = ({});
    if(!user_list) user_list = ({});
    foreach(string subdir in cre_dirs){
        cres += unguarded( (: get_dir,DIR_CRES+"/"+subdir+"/" :) );
    }
#if ENABLE_INSTANCES
    cres = filter(cres, (: grepp($1, ""+__PORT__) :) );
#else
    cres = filter(cres, (: sscanf($1, "%*s.%*s.o") == 1 :) );
#endif
    foreach(string cre in cres){
        string sub;
#if ENABLE_INSTANCES
        sub = replace_string(cre, "."+__PORT__+".o", "");
        sub = replace_string(sub, ".o","");
#else
        sub = replace_string(cre, ".o","");
#endif
        if(sub && !grepp(sub, ".") && 
                (member_array(sub, user_list) == -1 ||
                 member_array(sub, creators) == -1)){
            cache += ({ sub });
        }

        if(sizeof(cache) > 20){
            call_out("ScheduledPlayerAdd", i, cache);
            cache = ({});
        }
    }
    if(sizeof(cache)){
        call_out("ScheduledPlayerAdd", i, cache);
    }
    return cres;
}

string *CompilePlayerList(){
    string *play_dirs = unguarded( (: get_dir(DIR_PLAYERS+"/") :) );
    string *plays = ({});
    string *cache = ({});
    int i;
    players = ({});
    if(!user_list) user_list = ({});
    foreach(string subdir in play_dirs){
        plays += unguarded( (: get_dir,DIR_PLAYERS+"/"+subdir+"/" :) );
    }
#if ENABLE_INSTANCES
    plays = filter(plays, (: grepp($1, ""+__PORT__) :) );
#else
    plays = filter(plays, (: sscanf($1, "%*s.%*s.o") == 1 :) );
#endif
    foreach(string play in plays){
        string sub;
#if ENABLE_INSTANCES
        sub = replace_string(play, "."+__PORT__+".o", "");
        sub = replace_string(sub, ".o","");
#else
        sub = replace_string(play, ".o","");
#endif
        if(sub && !grepp(sub, ".") && 
                (member_array(sub, user_list) == -1 ||
                 member_array(sub, players) == -1)){
            cache += ({ sub });
        }

        if(sizeof(cache) > 20){
            call_out("ScheduledPlayerAdd", i, cache);
            cache = ({});
        }
    }
    if(sizeof(cache)){
        call_out("ScheduledPlayerAdd", i, cache);
    }
    return plays;
}

void create() {
    int ret;
    ::create();
    SaveFile = save_file(SAVE_PLAYER_LIST);
    ret = unguarded( (: RestoreObject, SaveFile :) );
    if(PendingEncres) PendingEncres = distinct_array(PendingEncres);
    if(PendingDecres) PendingDecres = distinct_array(PendingDecres);
    if(players) players = distinct_array(players);
    else players = ({});
    if(creators) creators = distinct_array(creators);
    else creators = ({});
    if(user_list) user_list = distinct_array(user_list);
    else user_list = ({});
    unguarded((: SaveObject, SaveFile :));
    if(!Levels) Levels = ([]);
    call_out( (: CompileLevelList :), 3);
    call_out( (: CompileCreList :), 1);
    call_out( (: CompilePlayerList :), 5);
}

string *eventCre(string str){
    str = lower_case(cleaned_name(str));
    if(member_array(str,creators) == -1) creators += ({ str });
    if(member_array(str,players) != -1) players -= ({ str });
    unguarded((: SaveObject, SaveFile :));
    return copy(creators);
}

string *eventDecre(string str){
    str = lower_case(str);
    if(member_array(str,creators) != -1) creators -= ({ str });
    if(member_array(str,players) == -1) players += ({ str });
    unguarded((: SaveObject, SaveFile :));
    return players + ({});
}

static int AutoAdvance(object ob, int level){
    int ret;
    ob->ChangeLevel(level);
    ret = ob->GetLevel();
    if(ret >= level){
        ob->eventPrint("%^RED%^%^B_BLACK%^You automatically advance to "+
                "level "+level+". Congratulations!%^RESET%^");
        ob->AddTrainingPoints(level);
        if(Levels[level]["title"]){
            ob->AddTitle(Levels[level]["title"]);
            ob->RemoveTitle(Levels[level-1]["title"]);
        }
    }
    return ret;
}

int CheckAdvance(object ob){
    int dlev, xp, qp;
    if(!ob || !playerp(ob)) return 0;
    if(!sizeof(Levels)) CompileLevelList();
    dlev = (ob->GetLevel())+1;
    xp = ob->GetExperiencePoints();
    qp = ob->GetQuestPoints();
    if(xp >= Levels[dlev]["xp"] && qp >= Levels[dlev]["qp"]){
        if(AUTO_ADVANCE) AutoAdvance(ob, dlev);
        return 1;
    }
    return 0;
}

void AddPlayerInfo(mixed arg){
    if(!objectp(arg) && !stringp(arg)){
        return ;
    }
    if(!creators) creators = ({});
    if(!players) players = ({});
    if(!user_list) user_list = ({});
    if(objectp(arg)){
        if(base_name(previous_object())!=LIB_CONNECT &&!interactive(arg)){
            return;
        }
        else player_save_file = base_name(arg)+".o";
    }
    else {
        arg = lower_case(arg);
        if(last(arg,2) != ".o") arg += ".o";
        if(!grepp(arg,"/")){
            string initial = arg[0..0];
            if(!directory_exists(DIR_ESTATES))
                mkdir(DIR_ESTATES);
            if(!directory_exists(DIR_ESTATES+"/"+initial))
                mkdir(DIR_ESTATES + "/"+initial);
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2));
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2)+"/tmp");
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2)+"/log");
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2)+"/bak");
            mkdir(DIR_ESTATES + "/"+initial+"/"+truncate(arg,2)+"/adm");
            player_save_file = DIR_CRES +"/"+initial+"/"+arg;
#if ENABLE_INSTANCES
            player_save_file = new_savename(player_save_file);
#endif
            if(!file_exists(player_save_file))
                player_save_file = DIR_PLAYERS +"/"+initial+"/"+arg;
#if ENABLE_INSTANCES
            player_save_file = new_savename(player_save_file);
#endif
            if(!file_exists(player_save_file)){
                return;
            }
        }
        else player_save_file = arg;
    }
#if ENABLE_INSTANCES
    player_save_file = old_savename(player_save_file);
    player_save_file = new_savename(player_save_file);
    namestr = replace_string(last_string_element(player_save_file,"/"),
            "."+__PORT__+".o","");
#else
    namestr = replace_string(last_string_element(player_save_file,"/"),".o","");
#endif
    namestr = cleaned_name(namestr);
    if(grepp(player_save_file, DIR_CRES) && 
            member_array(namestr,creators) == -1){ 
        creators += ({ namestr }) ; 
    }
    else if(member_array(namestr,players) == -1){ 
        players += ({ namestr }) ; 
    }
    if(!unguarded( (: file_exists, player_save_file :) )){
        return;
    }
    if(member_array(namestr,user_list) == -1){
        user_list += ({ namestr }) ; 
    }
    unguarded((: SaveObject, SaveFile :));
}

string *GetPlayerList(){
    return players + ({});
}

string *GetCreatorList(){    
    return copy(creators);
}

string *GetUserList(){
    return copy(user_list);
}

mapping GetLevelList(){
    if(!sizeof(Levels)) CompileLevelList();
    return copy(Levels);
}

string GetLevels(){
    if(!sizeof(LevelList)) CompileLevelList();
    return LevelList;
}

int RemoveUser(string str){
    if(!str || str == "") return 0;
    str = lower_case(str);
    if(user_exists(str)) return 0;
    if(member_array(str, players) != -1) players -= ({ str });
    if(member_array(str, creators) != -1) creators -= ({ str });
    if(member_array(str, user_list) != -1) user_list -= ({ str });
    return 1;
}

string *AddPendingEncre(string str){
    validate();
    if(!PendingEncres) PendingEncres = ({});
    if(str && str != "") PendingEncres += ({ lower_case(str) });
    unguarded((: SaveObject, SaveFile :));
    return copy(PendingEncres);
}

string *RemovePendingEncre(string str){
    validate();
    if(!PendingEncres) PendingEncres = ({});
    if(!str || str == "") return copy(PendingEncres);
    str = lower_case(str);
    if(member_array(str, PendingEncres) != -1) PendingEncres -= ({ lower_case(str) });
    unguarded((: SaveObject, SaveFile :));
    return copy(PendingEncres);
}

string *GetPendingEncres(){
    validate();
    if(!PendingEncres) PendingEncres = ({});
    return copy(PendingEncres);
}

string *AddPendingDecre(string str){
    validate();
    if(!PendingDecres) PendingDecres = ({});
    if(str && str != "") PendingDecres += ({ lower_case(str) });
    unguarded((: SaveObject, SaveFile :));
    return copy(PendingDecres);
}

string *RemovePendingDecre(string str){
    validate();
    if(!PendingDecres) PendingDecres = ({});
    if(!str || str == "") return copy(PendingDecres);
    str = lower_case(str);
    if(member_array(str, PendingDecres) != -1) PendingDecres -= ({ lower_case(str) });
    unguarded((: SaveObject, SaveFile :));
    return copy(PendingDecres);
}

string *GetPendingDecres(){
    validate();
    if(!PendingDecres) PendingDecres = ({});
    return copy(PendingDecres);
}

static int LoadPlayer(string str){
    string arg = "", initial = "";
    arg = last_string_element(lower_case(replace_string("/"+str,".o","")),"/");
#if ENABLE_INSTANCES
    arg = replace_string(arg, "."+__PORT__, "");
#endif
    if(!user_exists(arg)){
        return 0;
    }
    arg += ".o";
    initial = arg[0..0];
    player_save_file = DIR_CRES +"/"+initial+"/"+arg;
#if ENABLE_INSTANCES
    player_save_file = new_savename(player_save_file);
#endif
    if(!file_exists(player_save_file)){
        player_save_file = DIR_PLAYERS +"/"+initial+"/"+arg;
#if ENABLE_INSTANCES
        player_save_file = new_savename(player_save_file);
        if(!file_exists(player_save_file)){
            player_save_file = old_savename(player_save_file);
        }
#endif
    }
    if(!file_exists(player_save_file)){
        return -1;
    }
    RestoreObject(player_save_file,1);
    return 1;
}

int eventDestruct(){
    validate();
    unguarded((: SaveObject, SaveFile :));
    return ::eventDestruct();
}

static mixed GetVariable(string val){
    if(!val){
        return variables(this_object());
    }
    if(member_array(val,variables(this_object())) == -1){
        write("No such PLAYER_D variable exists.");
        return 0;
    }
    return fetch_variable(val);
}

mixed GetPlayerVariables(){
    string *vars = variables(this_object());
    vars -= ({"PlayerDataMap", "PendingEncres", "PendingDecres"});
    vars -= ({"players", "creators", "user_list" });
    vars -= ({"player_save_file", "namestr", "ob", "gplayer"});
    return vars;
}

static mixed GetPlayerVariable(string val){
    string vars = GetPlayerVariables();
    mixed ret;
    if(!val) return vars;
    if(member_array(val,variables(this_object())) == -1){
        write("No such player variable exists.");
        return 0;
    }
    ret = fetch_variable(val);
    return ret;
}

mixed GetPlayerData(string player, string val){
    mixed ret;
    validate();
    if(!player) return 0;
    if(!user_exists(player)){
        write("No such user exists.");
        return 0;
    }
    gplayer = player;
    if(ob = find_player(player)){
        unguarded( (: ob->save_player(gplayer) :));
    }
    unguarded((: SaveObject, SaveFile :));
    unguarded( (: LoadPlayer(gplayer) :) );
    ret = GetPlayerVariable(val);
    unguarded((: RestoreObject, SaveFile :));
    return ret;
}

string array GetAdminIPs(){
    string array ret_array, name_array, line_array;
    string config_file;
    config_file = read_file(CFG_GROUPS);
    line_array = explode(config_file, "\n");
    ret_array = ({});
    name_array = ({});
    foreach(string line in line_array){
        string g,m;
        if(sizeof(line)) line = trim(line);
        if(sscanf(line,"(%s)%s",g,m) != 2) continue;
        if(g == "SECURE") name_array += explode(trim(m),":");
        else if(g == "ASSIST") name_array += explode(trim(m),":");
    }
    if(!sizeof(name_array)) return name_array;
    foreach(string nombre in name_array){
        ret_array += ({ GetPlayerData(nombre,"HostSite") });
    }
    return ret_array;
}

int CheckBuilder(object who){
    if(creatorp(who)) return 1;
    if(builderp(who)){
        if(!directory_exists(homedir(who)+"/area")){
            unguarded( (: make_workroom($(who)) :) );
        }
        return 1;
    }
    return 0; 
}

string GetUserPath(mixed name){
    string ret = "/tmp/";
    if(!creators) creators = ({});
    if(!players) players = ({});
    if(name && objectp(name)) name = name->GetKeyName();
    if(!name){
        if(!this_player()) return ret;
        else name = this_player()->GetKeyName();
    }
    if(member_array(name, creators) != -1){
        ret = REALMS_DIRS+"/"+name+"/";
    }
    else if(member_array(name, players) != -1){
        ret = DIR_ESTATES + "/"+name[0..0]+"/"+name+"/";
    }
    return ret;
}

static void UserUpdate(string name, int status){
    object ob = find_player(name);
    if(member_array(name, local_users()) != -1) status = 1;
    else if(ob && !interactive(ob)) status = -1;
    else status = 0;
    INSTANCES_D->SendWhoUpdate(name, status);
}

void PlayerUpdate(string name, int status){
    call_out("UserUpdate", 1, name, status);
}
