/*    /lib/props/deterioriation.c
 *    From the Dead Souls Object Library
 *    Handles the deterioration of objects
 *    Created by Descartes of Borg 970101
 *    Version: @(#) deterioration.c 1.1@(#)
 *    Last modified: 97/01/01
 */
#include <daemons.h>

private int Broken          = 0;
private int DamagePoints    = 0;
private int Deterioration   = 0;
private int MaxDamagePoints = 20000;

// abstract methods
void eventDeteriorate(int type);
// end abstract methods

int GetBroken(){
    return Broken;
}

int SetBroken(int x){
    return (Broken = x);
}

int SetDamagePoints(int x){
    return (MaxDamagePoints = DamagePoints = x);
}

int GetDamagePoints(){
    return DamagePoints;
}

int GetDeterioration(){
    return Deterioration;
}

string GetItemCondition(){
    if( Deterioration ){
        return "It has worn down completely.";
    }
    else {
        return 0;
    }
}

string array GetSave(){
    return ({ "Broken", "DamagePoints", "Deterioration" });
}

int eventReceiveDamage(mixed agent, int type, int amt, int i, mixed array l){
    int x = -1;
    mixed worn = this_object()->GetWorn();
    mapping temp_prot = this_object()->GetProtectionMap();
    if(query_verb() == "pick") return 0;
    if(objectp(agent)){
        if(estatep(agent) && !estatep(this_object())) return 0;
        if(!estatep(agent) && estatep(this_object())) return 0;
    }
    if(sizeof(worn) && mapp(temp_prot) && sizeof(temp_prot)){
        foreach(int t, int val in temp_prot){
            if( t & type ){
                if( x == -1 || val < x ){
                    x = val;
                }
            }
        }
        x = x/2 + random(x/2);
        DamagePoints -= ((amt * 5) - x);
    }
    else DamagePoints -= (amt * 5);
    if( DamagePoints < 1 ){
        Deterioration++;
        DamagePoints = MaxDamagePoints;
        eventDeteriorate(type);
    }
    return x;
}

