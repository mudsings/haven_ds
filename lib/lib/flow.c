/* Eventually this will handle other kinds of flow
 * materials, but for now it is hardcoded just to be water.
 */

#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
#include <medium.h>

inherit LIB_FLASK;
mixed *neighbors = ({});
mixed *rooms = ({});
mixed *first = ({});
mixed *second = ({});
mixed *third = ({});
mixed *fourth = ({});
static int press;
static string FlowType = "water";
static string ThisFile;
static int HBOverride, orig_medium, DoorStopped = 1;

void shb(int i){
    if(!HBOverride) set_heart_beat(i);
}

void create(){
    ::create();
    SetKeyName("flow object");
    SetId( ({"flow","flood","object","water","overflow"}) );
    SetAdjectives( ({"flow","flood"}) );
    SetShort("a flood");
    SetLong("This is an overflow of water.");
    SetMass(20);
    SetInvis(1);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    shb(5);
    SetNoCondition(1);
    SetPreventGet("You can't get that.");
    if(FlowType == "water"){
        SetFlaskContents("water");
        SetFlaskUses(2);
        SetStrength(5);
        SetMaxFlask(2);
        SetEverFill(1);
        SetMealType(MEAL_DRINK);
    }
    ThisFile = base_name(this_object());
}

void eventFlood(mixed targets){
    object env = environment();
    if(!env) return;
    foreach(mixed room in scramble_array(targets)){
        mixed flooder;
        //tc("room: "+identify(room),"white");
        if(environment() == room) continue;
        if(press < 2) break;
        flooder = filter(all_inventory(room),
                (: base_name($1) == ThisFile :) );
        if(sizeof(flooder)){
            if(sizeof(flooder) > 1) flooder[1..]->eventDestruct();
            flooder = flooder[0];
        }
        else {
            int flood;
            flooder = new(ThisFile);
            //tc("flooder: "+identify(flooder),"green");
            flooder->SetProperty("LastLocation", base_name(env)); 
            flood = flooder->eventMove(room);
            if(!flood) continue;
            tell_room(room,"This area starts flooding with water!");
            //tc("flooder: "+identify(flooder)+", env: "+
            //identify(environment(flooder)),"cyan");
        }
        //tc("flooder","blue");
        if(flooder->GetPressure() < press){
            if(press && flooder->AddPressure(1)) press--;
            if(press < 2){ 
                tell_room(env,"The water recedes a bit.");
            }
        }
    }
}

void CheckRooms(){
    mixed ret = ({});
    object env;
    mapping Exits;
    env = room_environment();
    if(press < 1){
        eventDestruct();
        return;
    }
    if(clonep() && environment() && !living(environment()) && press > 1){
        shb(5);
    }
    if(env){
        Exits = env->GetExitMap(); 
        Exits += env->GetEnterMap(); 
        foreach(mixed key, mixed val in Exits){
            mixed door = env->GetDoor(key);
            int ok =1;
            if(door){
                shb(1);
                if(DoorStopped && door->GetClosed() && !door->GetPerforated()){
                    //tc("NOT OK");
                    ok = 0;
                }
                else{
                    //tc("door: "+identify(door)+", closed: "+door->GetClosed()+
                    //", perforated: "+door->GetPerforated(),"cyan");
                    //tc("key: "+identify(key)+", val: "+identify(val)+
                    //"env: "+identify(env),"white");
                }
            }
            if(ok) ret += ({ val });
        }
    }
    if(sizeof(ret)) rooms = ret;
    //tc("rooms: "+implode(rooms," "));
    if(press > 1){
        //tc("whee "+identify(this_object()),"red");
        rooms = ({});
        foreach(mixed key, mixed val in Exits){
            if(member_array(val, ret) == -1) continue;
            if(key == "down") first += ({ load_object(val) });
            else if(key == "up") fourth += ({ load_object(val) });
            else rooms += ({ load_object(val) });
        }
        //tc("rooms: "+identify(rooms),"yellow");
        foreach(mixed room in rooms){
            mixed flooder;
            //tc("room: "+identify(room),"white");
            if(press < 2) break;
            flooder = filter(all_inventory(room),
                    (: base_name($1) == ThisFile :) );
            if(!sizeof(flooder)) second += ({ room });
            else third += ({ room });
        }
        if(sizeof(first) && press > 0) eventFlood(first);
        if(sizeof(second) && press > 1) eventFlood(second);
        if(sizeof(third) && press > 1) eventFlood(third);
        if(sizeof(fourth) && press > 2) eventFlood(fourth);
    }
}

void heart_beat(){
    object env = environment();
    if(!clonep() || !env || living(env) || press < 2){
        shb(60);
        return;
    }
    CheckRooms();
    if(press > 1 && env){
        env->SetMedium(MEDIUM_WATER);
    }
    else if(env){
        env->SetMedium(orig_medium);
    }
}

int AddPressure(int x){
    int i;
    object env = environment();
    if(!env) return 0;
    i = env->GetFlowLimit();
    if(i && x > 0 && press >= i) return 0;
    if(x){
        if(press == 1 && x > 0){
            tell_room(env,"The area completely fills with water!");
        } 
        if(press > 1 && x < 0){
            tell_room(env,"The water recedes a bit.");
        }
        press += x;
    }
    if(press) shb(5);
    return press;
}

int GetPressure(){
    return press;
}

int SetPressure(int i){
    press = i;
    return press;
}

int GetHBOverride(){
    return HBOverride;
}

int SetHBOverride(int x){
    if(x) HBOverride = 1;
    else HBOverride = 0;
    return HBOverride;
}

int GetDoorStopped(){
    return DoorStopped;
}

int SetDoorStopped(int i){
    DoorStopped = i;
    return DoorStopped;
}

void init(){
    ::init();
}

string GetRoomAffectLong(){
    if(press > 1){
        return "\n%^CYAN%^This area is completely filled with water!%^RESET%^";
    }
    if(press == 1){
        return "\n%^BOLD%^%^BLUE%^This area is flooded with water.%^RESET%^";
    }
    return "";
}

int eventDestruct(){
    object env = environment();
    if(env){
        env->SetMedium(orig_medium);
    }
    return ::eventDestruct();
}

int eventMove(mixed dest){
    object env;
    int ret = ::eventMove(dest);
    if(ret && env = room_environment(this_object())){
        orig_medium = env->GetMedium();
    }
    return ret;
}

