#include <lib.h>
#include <dirs.h>
#include <virtual.h>
#define __DIR__ "/domains/town/virtual/"

inherit LIB_VIRT_LAND;

static private int XPosition, YPosition;

varargs void SetLongAndItems(int x, int y, int z);

varargs int LimitTravel(int requested, int maximum, int lessthan){
    if(lessthan && requested < maximum) return maximum;
    else if(lessthan && requested > maximum) return requested;
    else if(requested > maximum) return maximum;
    else return requested;
}

varargs static void create(int x, int y) {
    string n, s, e, w;
    string ne, nw, se, sw;
    string fly;

    int max_north = 25;
    int max_south = 1;
    int max_east = 25;
    int max_west = 1;

    SetNoReplace(1);
    virt_land::create();
    XPosition = x;
    YPosition = y;
    SetClimate("temperate");
    SetAmbientLight(30);
    SetLongAndItems(x, y);
    SetShort("a thick forest");
    if( x == max_east ) e = "forest/" + (x) + "," + y;
    else e = "forest/" + (x+1) + "," + y;
    if( x == max_west ) w = "forest/" + (x) + "," + y;
    else w = "forest/" + (x-1) + "," + y;
    if( y == max_north ) n = "forest/" + x + "," + (y);
    else n = "forest/" + x + "," + (y+1);
    if( y == max_south ) s = "forest/" + x+ "," + (y);
    else s = "forest/" + x + "," + (y-1);
    fly = "sky/" + x + "," + y + "," + 1;

    nw = "forest/" + LimitTravel(x - 1, max_west, 1) + "," + LimitTravel(y+1, max_north);
    ne = "forest/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y+1, max_north);
    sw = "forest/" + LimitTravel(x - 1, max_west, 1) + "," + LimitTravel(y-1, max_south, 1);
    se = "forest/" + LimitTravel(x + 1, max_east) + "," + LimitTravel(y-1, max_south, 1);

    SetFlyRoom(__DIR__+fly);

    SetGoMessage("You can't travel in that direction.");
    if( n ) AddExit("north", __DIR__ + n);
    if( s ) AddExit("south", __DIR__ + s);
    if( e ) AddExit("east", __DIR__ + e);
    if( w ) AddExit("west", __DIR__ + w);
    if( ne ) AddExit("northeast", __DIR__ + ne);
    if( nw ) AddExit("northwest", __DIR__ + nw);
    if( se ) AddExit("southeast", __DIR__ + se);
    if( sw ) AddExit("southwest", __DIR__ + sw);

    if(x == 24 && y == 1){
        RemoveExit("south");
        AddExit("south","/domains/town/room/forest_path1");
    }
    else if(y == 1){
        RemoveExit("south");
        RemoveExit("southeast");
        RemoveExit("southwest");
    }
    if(x == 24 && y == 25){
        RemoveExit("north");
        AddExit("north","/domains/town/room/cave_entrance");
    }
    else if(y == 25){
        RemoveExit("north");
        RemoveExit("northeast");
        RemoveExit("northwest");
    }
    if(x == 1){
        RemoveExit("west");
        RemoveExit("northwest");
        RemoveExit("southwest");
    }
    if(x == 25){
        RemoveExit("east");
        RemoveExit("northeast");
        RemoveExit("southeast");
    }


}

varargs void SetLongAndItems(int x, int y, int z) {
    mapping inv, items;
    string str;
    ::SetLongAndItems(x, y, z);

    inv = ([]);
    str = "You are in a vast forest. The trees and "
    "thick vegetation press in from all sides, appearing almost "
    "threatening. "
    "No path marks your way. ";
    if(query_night()) str += "The stars of the night sky "
        "are barely visible through the heavy forest canopy overhead.";
    else str+= "Sunlight penetrates the tree cover with great "
        "difficulty, rendering the forest cool and dark.";
    if(x == 25) str+= " Travel east is made impossible by a river "
        "rushing north to south here. On the other side of the river "
        "a great slope rises, and it appears that on top of it is "
        "a road that leads north to a high plateau.";
    if(y == 25) str+= " A steep cliff rises north of here, making "
        "travel north into the mountains impossible.";
    if(x == 1) str += " The forest is impassably thick to the west.";
    if(y == 1) str += " The forest is impassably thick southward.";
    if(x == 24 && y == 1) str += "\n%^GREEN%^There is a sign here you can read.%^RESET%^";
    if(x == 24 && y == 25) str += "\n%^GREEN%^There is a cave entrance in the cliff wall.%^RESET%^";
    SetItems( ([ "forest" : "It is so vast.",
        ({"woods","trees","vegetation","plants"}) : "Thick, foreboding and "
        "oppressive, these seem to add to an air of danger and claustrophobia.", 
        "no path" : "You observe the presence of its absence.",
        ({"mountain","mountains"}): "The snow capped peaks of the great "
        "northern range are faintly visible from here.",
        "cliff" : "To the north, a high cliff rises from the forest.",
        ({"canopy","forest canopy","heavy forest canopy"}) : "The great "
        "branches and heavy leaves of the trees here form the forest "
        "canopy, almost a ceiling through which little light can pass.",
      ]) );
    if(y == 25) {
        AddItem( ({ "river", "stream", "great river"}), 
          "This narrow but powerful river presents an insurmountable "
          "obstacle to further travel east." );
        AddItem( ({ "slope","road","plateau"}), "Looks like that's "
          "Fort Road, high on a slope and running north to the Fortress "
          "on the Frontiers.");
    }
    if(x == 24 && y == 25) {
        AddItem(({"cave","opening","entrance","cave entrance"}) , 
          "This is a rather scary looking opening in the cliff wall, leading "
          "north into the ground.");
        AddEnter("cave","/domains/town/room/cave_entrance");
        SetObviousExits("n, s, e, w, se, sw, enter cave");
    }

    if(x == 24 && y == 1) {
        AddItem("sign" , "This is a hastily-lettered sign planted on the ground.");
        SetRead( ({"sign"}) , "Beware th");
    }
    if( !random(50) ) {
        str += "  Burnt wood, scattered rocks and twigs, and other signs "
        "of an abandoned camp site are scattered about.";
        AddItem( ({ "twigs", "sticks", "kindling", "wood", "burnt wood" }) , 
          "Though long since burnt to nothing, scattered kindling "
          "and burnt wood lie about as a memory of travellers who have "
          "passed through");
        if( random(2) ) {
            string thing;

            foreach(thing in ({ "twigs", "sticks", "kindling", "wood" })) 
            SetSearch(thing, function(object who, string str) {
                  object ob;
                  string thing2;

                  if( !(ob = new("/domains/Ylsrim"+ "/etc/pole")) )
                      return 0;
                  who->eventPrint("You find a fishing pole!");
                  eventPrint((string)who->GetName() + " finds a fishing pole "
                    "among the abandoned campsite.", who);
                  foreach(thing2 in ({ "twigs", "sticks", "kindling", "wood"}))
                  RemoveSearch(thing2);
                  if( !((int)ob->eventMove(this_player())) ) {
                      who->eventPrint("You drop the pole!");
                      eventPrint((string)who->GetName() + " drops the pole.",
                        who);
                      ob->eventMove(this_object());
                  }
                  return;
                });
          }
        }
        else if( !random(10) ) 
            SetSmell("default", "You smell a distant camp fire.");
        if( !random(25) )
            inv["/domains/town/npc/forest_orc"] = random(3);
        if( !random(45) )
            inv["/domains/town/npc/bear"] = 1;
        if( !random(15) )
            inv["/domains/town/npc/deer"] = 1;
        if( !random(65) )
            inv["/domains/town/npc/gecko"] = 1;
        if( !random(35) )
            inv["/domains/town/npc/newt"] = 1;
        if( !random(75) )
            inv["/domains/town/npc/spider"] = 1;

        else if( !random(14) ) 
            SetListen("default", "You hear voices in the distance.");
        //if(x == 24 && y == 1) inv["/domains/town/obj/sign"] = 1;
        SetLong(str);
        SetDayLight(25);
        SetNightLight(0);
        SetInventory(inv);
    }
