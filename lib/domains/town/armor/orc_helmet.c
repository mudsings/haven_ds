#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

int CheckOrc();
static void create(){
    armor::create();
    SetKeyName("orc helmet");
    SetId(({"helmet"}));
    SetAdjectives(({"bear","bone","skull","orc"}));
    SetShort("a bear skull helmet");
    SetLong("This is a helmet made of the hard bone of a bear, "+
      "treated with some sort of thick resin to "+
      "prevent shattering.");
    SetMass(180);
    SetBaseCost("silver",100);
    SetDamagePoints(100);
    SetArmorType(A_HELMET);
    SetProtection(BLUNT, 20);
    SetProtection(BLADE, 20);
    SetProtection(KNIFE, 20);
    SetWear( (: CheckOrc :) );
}

int CheckOrc() {
    if( (string)this_player()->GetRace() == "orc" ) {
	write("You can almost feel the power of the bear as you wear its skull.");
	say((string)this_player()->GetName() + " wears a bear skull helmet.");
	return 1;
    }
    else {
	write("The helmet appears designed for orc anatomy. It does not fit you.");
	return 1;
    }
}
