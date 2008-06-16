/*    /cmds/creator/stat.c
 *    from the Dead Souls LPC Library
 *    a command to view something's stats
 *    created by Descartes of Borg 950409
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string *lines, *arr, *limbs;
    object ob;
    string tmp1, tmp2, gold;
    int i, x, y, cols;

    if( args == "" || !args || args == "me" ) args = this_player()->GetKeyName();
    else if(args && !creatorp(this_player())) {
        write("You can only stat yourself.");
        return 1;
    }
    if(!environment(this_player())){
        write("You have no environment. Your stats are the least of your worries.");
        return 1;
    }
    if( !(ob = present(args, environment(this_player()))) )
        if( !(ob = find_player(convert_name(args))) &&
          !(ob = find_living(lower_case(args))) )
            return capitalize(args) + " is nowhere to be found.";
    if(!living(ob)) return capitalize(args) + " is not alive.";
    cols = ((int *)this_player()->GetScreen())[0];
    tmp1 = (string)ob->GetCapName() + " aka " + (string)ob->GetShort() +
    ", level " + (int)ob->GetLevel() + " " + (string)ob->GetGender();
    if( !(tmp2 = (string)ob->GetRace()) ) tmp2 = "blob";
    tmp1 += " " + tmp2;
    if( !(tmp2 = (string)ob->GetClass())  || !stringp(tmp2)) tmp2 = "drifter";
    tmp1 += " " + capitalize(tmp2);
    if( tmp2 = (string)ob->GetSpouse() )
        tmp1 += " (spouse: " + tmp2 + ")";
    lines = ({ center(tmp1, cols) });
    if( (int)ob->GetUndead() ) tmp1 = "%^BOLD%^RED%^UNDEAD%^RESET%^";
    else tmp1 = "%^BOLD%^GREEN%^Alive%^RESET%^";
    if( (int)ob->GetSleeping() ) tmp1 += " / Sleeping";
    else tmp1 += " / Awake";
    if( (int)ob->GetParalyzed() ) tmp1 += " / Paralyzed";
    lines += ({ center(tmp1, cols), "" });
    lines += ({ center("Health: " +(int)ob->GetHealthPoints() + "/"+
        (int)ob->GetMaxHealthPoints() + "   Magic: " +
        (int)ob->GetMagicPoints() + "/" +
        (int)ob->GetMaxMagicPoints() + "   Stamina: " +
        (int)ob->GetStaminaPoints() + "/" +
        to_int((float)ob->GetMaxStaminaPoints()) + "   Carry: " +
        (int)ob->GetCarriedMass() + "/" +
        (int)ob->GetMaxCarry(), cols) });
    lines += ({ center("Food: " + (int)ob->GetFood() + "    " +
        "Drink: " + (int)ob->GetDrink() + "    " +
        "Alcohol: " + (int)ob->GetAlcohol() + "    " +
        "Caffeine: " + (int)ob->GetCaffeine() + "    " +
        "Poison: " + (int)ob->GetPoison() + "    ", cols) });
    lines += ({ "\n" }) ;
    lines += ({ center("Training Points: " + (int)ob->GetTrainingPoints() +
        "    " +
        "Quest Points: "+ (int)ob->GetQuestPoints() +
        "    " +
        //Fix below courtesy of Jonez
        "Experience Points: "+ (int)ob->GetExperiencePoints()),cols 
    });
    lines += ({ "", "Limbs:" });
    limbs = (string *)ob->GetWieldingLimbs();
    if(ob && !ob->GetGhost()) arr = map((string *)ob->GetLimbs(),
          (: sprintf("%:-14s%s (%d) %d/%d", $1,
              ((member_array($1, $(limbs)) == -1) ? " " : "*"),
              (int)($(ob))->GetLimbClass($1),
              (int)($(ob))->GetHealthPoints($1),
              (int)($(ob))->GetMaxHealthPoints($1)) :));
    i = sizeof(arr);
    while(i--) if( (y = strlen(arr[i])) > x ) x = y;
    x = cols/(x+2);
    lines += explode(format_page(arr, x), "\n") + ({ "", "Skills:" });
    arr = map((string *)ob->GetSkills(),
      function(string skill, object who) {
          mapping mp = (mapping)who->GetSkill(skill);
          int x, max;
          x = to_int(percent(mp["points"],
              (int)who->GetMaxSkillPoints(skill, mp["level"])));
          max = ( mp["class"] == 1 ? 2 : 1 ) +  who->GetLevel();
          max *= 2;
          if( max < mp["level"] ) max = mp["level"];
          return sprintf("%:-18s (%d) %:2d%% - %d/%d",
            skill, mp["class"], x, mp["level"], max);
      }, ob);
    i = sizeof(arr);
    while(i--) if( (y = strlen(arr[i])) > x ) x = y;
    x = cols/(x+2);
    lines += explode(format_page(arr, x), "\n") + ({ "", "Stats:" });
    arr = map((string *)ob->GetStats(),
      (: sprintf("%:-12s (%d) %d/%d", $1,
          (int)($(ob))->GetStatClass($1),
          (int)($(ob))->GetStatLevel($1),
          (int)($(ob))->GetBaseStatLevel($1)) :));
    i = sizeof(arr);
    x = 0;
    while(i--) if( (y = strlen(arr[i])) > x ) x = y;
    x =cols/(x+2);
    lines += explode(format_page(arr, x), "\n");
    if(sizeof(ECONOMY_D->__QueryCurrencies())){
        if(valid_currency("gold")) gold = "gold";
        else gold = ECONOMY_D->__QueryCurrencies()[0];
        lines += ({ "", (string)ob->GetName()+" has amassed a net worth of " +
          ( (int)ob->GetNetWorth(gold) ) + " "+gold+"."});
        arr = filter( map((string *)ob->GetCurrencies(),
            (: ($(ob))->GetCurrency($1) &&
              sprintf("%d %s", ($(ob))->GetCurrency($1), $1) :)),
          (: $1 :));
        lines += ({ "Money on hand: "+implode(arr, ", ") });
    }
    this_player()->eventPage(lines, "system");
    return 1;
}

string GetHelp(string blah) {
    return ("Syntax: stat <living>\n\n"
      "Displays statistical information of a living object.");
}
