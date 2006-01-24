#include <lib.h>

inherit LIB_SENTIENT;

static void create() {
    sentient::create();
    SetKeyName("horse");
    SetId(({"horse","large horse"}));
    SetShort("a large horse");
    SetLong("A large, hooved quadruped, taller and more muscular than you.");
    SetLevel(5);
    SetRace("horse");
    SetGender("male");
    SetMelee(1);
    SetAction(5, ({ 
	"The horse snorts.", "You hear the horse whinny.", 
	"The horse flips its tail in the air.",
	"The horse looks around."}));
}
