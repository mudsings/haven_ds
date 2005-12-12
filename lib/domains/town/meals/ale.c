#include <lib.h>
#include <meal_types.h>

inherit LIB_MEAL;

static void create() {
    meal::create();
    SetKeyName("ale");
    SetId( ({ "bottle", "ale", "first class ale", "first-class ale" }) );
    SetShort("a bottle of ale");
    SetLong("A nice bottle of ale.");
    SetMass(60);
    SetMealType(MEAL_DRINK | MEAL_ALCOHOL);
    SetStrength(5);
    SetMealMessages("You drink a bottle of ale.",
      "$N drinks a bottle of ale.");
    SetEmptyName("bottle");
    SetEmptyShort("an empty bottle of ale");
    SetEmptyLong("It is an empty brown bottle that once held some ale.");
    SetBaseCost("silver",10);
}
