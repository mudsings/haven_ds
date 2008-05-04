#include <lib.h>
#include "include/round.h"
#include <vendor_types.h>
inherit LIB_ITEM;
private int caliber,millimeter;
private string pistoltype,ammotype;
private string rifletype;
void create(){
    ::create();
    SetKeyName("round");
    SetId(({"round","bullet"}));
    SetShort("a generic pistol round");
    SetLong("This is a generic pistol round.");
    SetMass(1);
    SetValue(1);
    SetVendorType(VT_TREASURE);
}
int SetRifleType(string str){rifletype=str; return 1; }
int SetPistolType(string str){pistoltype=str; return 1; }
int SetAmmoType(string str){ammotype=str; return 1; }
int SetCaliber(int x){ caliber=x; return 1; }
int SetMillimeter(int x){ millimeter=x; return 1; }
int GetMillimeter(){ return millimeter; }
int GetCaliber(){ return caliber; }
string GetRifleType(){ return rifletype; }
string GetPistolType(){ return pistoltype; }
string GetAmmoType(){ return ammotype; }
