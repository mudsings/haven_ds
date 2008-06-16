#include <lib.h>
inherit LIB_ROOM;

int ReadList(){
    write("AVAILABLE SPELLS:\n\n"
      "\tfireball, cost 1000:\tput the hurt on an opponent\n"
      "\tmissile, cost 500:\tCheaper and easier to learn than fireball\n"
      "\tbuffer, cost 200:\tconjure an invisible shield around you\n"
      "\tmeditate, cost 500:\tconjure an aura that speeds stamina recovery\n"
      "\n"
      "Example: If you want to learn the buffer spell:\n\n"
      "\"ask herkimer to teach buffer\"\n"
      "\n"
      "Please note that the spell may fail many times before "
      "you have enough practice and experience to cast "
      "it properly. All sales final.\n"

    );
    return 1;
}
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The Mages' Guild");
    SetLong("This is the local mage's guild. Here you can ask "+
      "the guildmaster to let you join, or to teach you spells. "+
      "For example, you may \"ask herkimer to join\" or "+
      "\"ask herkimer to teach missile\". The magic shop "+
      "is north."
      "\n%^GREEN%^There is a list of spells you can read.%^RESET%^");
    SetItems( ([
        ({"list","list on the wall"}) : "A list of available spells",
      ]) );
    SetInventory(([
        "/domains/town/npc/herkimer" : ({60, 1}),
      ]));
    SetRead(({"list","list on wall"}) , (: ReadList :) );
    SetExits( ([
        "north" : "/domains/town/room/magic_shop"
      ]) );

}
void init(){
    ::init();
}
