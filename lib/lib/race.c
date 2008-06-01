/*    /lib/race.c
 *    from the Dead Souls LPC Library
 *    handles all race specific management
 *    created by Descartes of Borg 950122
 *    Version: @(#) race.c 1.8@(#)
 *    Last modified: 96/11/11
 */

#include <lib.h>
#include <config.h>
#include <daemons.h>
#include <armor_types.h>
#include <damage_types.h>
#include <meal_types.h>
#include <medium.h>
#include <respiration_types.h>
#include "include/race.h"

inherit LIB_BODY;
inherit LIB_GENETICS;
inherit LIB_LANGUAGE;
inherit LIB_TALK;

private string Town, Race, Gender;
private static int Bulk, Respiration;

int GetRespiration(){
    int resp = RACES_D->GetRaceRespirationType(this_object()->GetRace());
    if(Respiration) return Respiration;
    return resp;
}

int SetRespiration(int i){
    return Respiration = i;
}

varargs int CanBreathe(object what, object where){
    object env = environment(this_object());
    int medium, restype; 

    if(this_object()->GetGodMode()) return 1;
    if(env && living(env)) env = environment(env);
    if(!env) return 0;
    medium = env->GetMedium();
    restype = this_object()->GetRespiration();

    //tc("restype: "+restype);
    //tc("medium: "+medium);

    if(restype & R_VACUUM) return 1;

    if((medium == MEDIUM_AIR || medium == MEDIUM_LAND || 
        medium == MEDIUM_SURFACE) && (restype & R_AIR) ) return 1;

    //tc("hmm.");

    if((medium == MEDIUM_WATER || medium == MEDIUM_SURFACE)
      && (restype & R_WATER) ) return 1;

    //tc("hmm2");

    if( medium == MEDIUM_METHANE && (restype & R_METHANE) ) return 1;

    //tc("hmm3");

    return 0;
}

// abstract methods
int GetParalyzed();
// end abstract methods

static void create(){
    body::create();
    genetics::create();
    Race = "blob";
    Gender = "neuter";
    Town = "Town";
}

mixed CanDrink(object ob){
    int strength, type;

    if( !ob ) return 0;
    strength = (int)ob->GetStrength();
    type = (int)ob->GetMealType();
    if( (type & MEAL_ALCOHOL) && ((strength + GetAlcohol()) >
        GetStatLevel("durability")) )
        return "That drink is too strong for you right now.";
    if( (type & MEAL_CAFFEINE) && ((strength + GetCaffeine()) >
        GetStatLevel("durability")) )
        return "That is too much caffeine for you right now.";
    if( (type & MEAL_DRINK) && ((strength + GetDrink()) > 100) )
        return "You can't drink any more fluids right now.";
    return 1;
}

mixed CanEat(object ob){
    if( ((int)ob->GetStrength() + GetFood()) > 100 )
        return "This is more food than you can handle right now.";
    else return 1;
}

varargs int eventDie(mixed agent){
    int x;

    if( (x = body::eventDie(agent)) != 1 ) return x;
    return 1;
}

mixed eventDrink(object ob){
    int type, strength;

    type = (int)ob->GetMealType();
    strength = (int)ob->GetStrength();
    if( type & MEAL_POISON ) AddPoison(strength);
    if( type & MEAL_DRINK ) AddDrink(strength);
    if( type & MEAL_ALCOHOL ) AddAlcohol(strength);
    if( type & MEAL_CAFFEINE ) AddCaffeine(strength);
    return 1;
}

mixed eventEat(object ob){
    AddFood((int)ob->GetStrength());
    if( (int)ob->GetMealType() & MEAL_POISON )
        AddPoison((int)ob->GetStrength());
    return 1;
}

varargs string SetRace(string race, mixed extra){
    mixed array args = allocate(5);
    mixed array tmp;
    mixed mixt;

    RACES_D->SetCharacterRace(race, args);

    switch(race){
    case "tree" : this_object()->SetBodyComposition("wood");break;
    case "balrog" : this_object()->SetBodyComposition("stone");break;
    case "elemental" : this_object()->SetBodyComposition("stone");break;
    case "golem" : this_object()->SetBodyComposition("clay");break;
    case "plant" : this_object()->SetBodyComposition("vegetation");break;
    }

    if(sizeof(args[4])){
        foreach(mixed key, mixed val in args[4]){
            this_object()->AddSkill(key,atoi(val[1]),atoi(val[0]));
        }
    }

    foreach(tmp in args[0]){
        mixt = copy(args[0]);
        SetResistance(tmp...);
    }
    foreach(tmp in args[1]){
        mixt = copy(args[1]);
        AddStat(tmp...);
    }
    if( stringp(args[2]) ){
        mixt = copy(args[2]);

        if(!ENGLISH_ONLY){
            SetLanguage(args[2], 100, 1);
        }
        else {
            SetLanguage("English", 100, 1);
        }
    }
    if( sizeof(args[3]) == 2 ){
        mixt = copy(args[3]);

        SetLightSensitivity(args[3]...);
    }
    if( extra != 1 ) NewBody(race);
    tmp = this_object()->GetId();
    if(tmp && member_array(race,tmp) == -1){
        tmp += ({ race });
        this_object()->SetId(tmp);
    }
    if( stringp(extra) ) return (Race = extra), race; 
    else return (Race = race);
}



string GetRace(){ return Race; }

string SetGender(string gender){ return (Gender = gender); }

string GetGender(){ return Gender; }

varargs void SetStat(string stat, int level, int classes){
    int healthPoints;

    genetics::SetStat(stat, level, classes);
    switch(stat){
    case "durability":
        eventCompleteHeal(healthPoints = GetMaxHealthPoints());
        eventHealDamage(healthPoints);
        break;
    case "intelligence":
        AddMagicPoints(GetMaxMagicPoints());
        break;
    case "agility":
        AddStaminaPoints(GetMaxStaminaPoints());
        break;
    }
}

varargs int GetMaxHealthPoints(string limb){
    int ret = 1;
    if(!limb) ret = ( 50 + (GetStatLevel("durability") * 10) );
    else {
        int x;

        x = GetLimbClass(limb);
        if(!x) x = 5;
        ret = ( (1 + GetStatLevel("durability")/x) * 10 );
    }
    if(ret < 1) return 1;
    else return ret;
}

int GetMaxMagicPoints(){
    return ( 50 + (GetStatLevel("intelligence") * 10) );
}

float GetMaxStaminaPoints(){
    return (50.0 + (GetStatLevel("agility") * 10.0) );
}

void NewBody(string race){
    mixed array args = allocate(2);
    mixed array tmp;

    body::NewBody(race);
    if(!race) return;
    RACES_D->SetCharacterLimbs(race, args);
    foreach(tmp in args[0]) AddLimb(tmp...);
    foreach(tmp in args[1]) AddFingers(tmp...);
}

string SetTown(string str){ return (Town = str); }

string GetTown(){ return Town; }

string GetResistance(int type){ return genetics::GetResistance(type); }

int GetLuck(){
    int x;

    x = random(GetStatLevel("luck")) / 20;
    x = ((x > 4) ? 4 : x);
    if( newbiep() ) x += random(7);
    return (x + random(4));
}

int GetMobility(){
    int max = GetMaxCarry();
    int encum, mob;

    if( GetParalyzed() ){
        return 0;
    }
    if( max < 1 ){
        max = 1;
    }
    encum = (GetCarriedMass() * 100)/max;
    encum -= (encum * this_object()->GetStatLevel("agility"))/200;
    mob = 100 - encum;
    if( mob > 100 ){
        mob = 100;
    }
    else if( mob < 1 ){
        mob = 0;
    }
    return mob;
}

int GetCarriedMass(){ return 0; }

int GetMaxCarry(){ 
    int carry_max;
    carry_max = this_object()->GetLivingMaxCarry();
    if(carry_max) return carry_max;
    else return ((2 + this_object()->GetStatLevel("strength")) * 50); 
}

int GetHeartRate(){
    int x, y;
    x = body::GetHeartRate();
    y = GetStatLevel("speed");
    if( y > 80 ) x -= 2;
    else if( y > 60 ) x -= 1;
    else if( y > 40 ) x = x;
    else if( y > 20 ) x += 1;
    else x += 2;
    if( x > 6 ) x = 6;
    else if( x < 1 ) x = 1;
#ifdef FAST_COMBAT
    if(FAST_COMBAT && this_object()->GetInCombat()) return 1;
#endif
    return x;
}

int GetHealRate(){
    int x;

    x = body::GetHealRate() + random((GetStatLevel("durability")/40) + 1);
    if( x > 6 ) x = 6;
    return x;
}

int GetStatLevel(string stat){ return genetics::GetStatLevel(stat); }

int GetAlcohol(){ return body::GetAlcohol(); }

static void heart_beat(){
    body::heart_beat();
    language::heart_beat();
    genetics::heart_beat();
    set_heart_beat(GetHeartRate());
}
