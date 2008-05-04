/*    /lib/props/equip.c
 *    From the Dead Souls Object Library
 *    Handles the property of being something that is equipped to a limb
 *    The word worn is used instead of equipped for historical reasons
 *    Created by Descartes of Borg 970101
 *    Version: @(#) equip.c 1.1@(#)
 *    Last modified: 97/01/01
 */

#include <armor_types.h>

private int          ArmorType  = A_WEAPON;
private string array Worn        = 0;

// abstract methods
string GetDefiniteShort();
// end abstract methods

int GetArmorType(){
    return ArmorType;
}

int SetArmorType(int x){
    if( !intp(x) ){
        error("Bad argument to SetArmorType().\n\tExpected value from "
          "/include/armor_types.h, Got: " + typeof(x) + "\n");
    }
    return (ArmorType = x);
}

string array GetSave(){
    return ({ "Worn" });
}

string array GetWorn(){
    return Worn;
}

static string array SetWorn(string array limbs){
    return (Worn = limbs);
}

mixed CanEquip(object who, string array limbs){
    if( GetWorn() ){
        return "#You are already using " + GetDefiniteShort() + ".";
    }
    return who->CanWear(this_object(), limbs);
}

mixed CanUnequip(object who){
    if( !GetWorn() ){
        return "#You are not using " + GetDefiniteShort() + ".";
    }
    return who->CanRemoveItem(who, this_object());
}

mixed eventEquip(object who, string array limbs){
    mixed tmp = who->eventWear(this_object(), limbs);

    if( tmp != 1 ){
        return tmp;
    }
    SetWorn(limbs);
    return 1;
}

static void eventRestoreEquip(string array limbs){
    mixed tmp;

    if( !limbs || !environment() ){
        return;
    }
    tmp = CanEquip(environment(), limbs);
    if( tmp == 1 ){
        tmp = eventEquip(environment(), limbs);
    }
    if( stringp(tmp) ){
        environment()->eventPrint(tmp);
    }
}

mixed eventUnequip(object who){
    mixed tmp = who->eventRemoveItem(this_object());

    if( tmp != 1 ){
        return tmp;
    }
    SetWorn(0);
    return 1;
}
