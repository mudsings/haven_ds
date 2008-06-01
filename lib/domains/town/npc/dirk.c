#include <lib.h>

inherit LIB_SENTIENT;

int hint;

int AdvanceDude(mixed arg);
mapping advancement;
int TalkFunc(){
    string thing1, thing2, thing3, thing4, thing5;

    thing1 = "There's no shame in being wimpy. Live to fight another day. Death takes away valuable xp.";
    thing2 = "Let me know if you see Princess Daphne.";
    thing3 = "Don't fight drunk.";
    thing4 = "Learning spells from Herkimer is a good idea.";
    thing5 = "Food, drink, and caffeine help restore health and strength.";

    switch(hint){
    case 0 : eventForce("say "+thing1);break;
    case 1 : eventForce("say "+thing2);break;
    case 2 : eventForce("say "+thing3);break;
    case 3 : eventForce("say "+thing4);break;
    case 4 : eventForce("say "+thing5);break;
    default :  eventForce("smile");
    }

    if(hint == 4) hint = 0;
    else hint++;
    return 1;
}

static void create() {
    sentient::create();
    SetKeyName("dirk");
    SetId(({"dirk"}));
    SetShort("Dirk the Tired");
    SetLong("For 20 years, Dirk the Daring has been trying "
      "to defeat the vile dragon Singe, with almost no respite. "
      "Today he just wants to rest and relax, and has accepted "
      "the position of the town's Adventurers' Guild master. "
      "If you feel you deserve it, \"ask dirk to advance\".");
    SetPolyglot(1);
    SetInventory(([
        "/domains/town/armor/collar" : "wear collar",
      ]));
    SetLevel(15);
    SetRace("human");
    SetGender("male");
    //SetAction(5, (: TalkFunc :));
    AddTalkResponse("hello", "hi! Ask me for a tip!");
    AddTalkResponse("hi", "hi! Ask me for a tip!");
    SetCommandResponses( ([ 
        "advance": (: AdvanceDude :) 
      ]) );
    SetRequestResponses( ([
        ({ "a hint", "hints", "a clue", "clues", "a tip", "tips" }) : 
        (: TalkFunc :),
      ]) );
    SetConsultResponses( ([
        ({ "level", "levels", "leveling", "advancement", "advancing" }) :
        "To level, get some experience out there and then come back "+
        "and ask me to advance. After a certain level you'll need "+
        "some quest points to advance, not just experience.",
        ({ "xp", "XP", "experience" }) : "You can score experience "+
        "points by killing monsters or completing some quests. "+
        "Sometimes you'll get xp for completing some task you didn't "+
        "even know would give you points. Generally though, it's combat "+
        "that results in XP rewards, if you win.",
        ({ "points" }) : "It's how to keep track of your progress. The "+
        "kinds of points I care about are experience points and "+
        "quest points.",
        ({ "quests", "quest", "quest points" }) : "Quests are missions "+
        "you can try to complete that will usually reward you with "+
        "quest points if you solve them. "+
        "You'll need quest points to advance past a "+
        "certain level.",
      ]) );
    advancement = ([ 
      1:(["title":"the utter novice","xp":0,"qp":0]),
      2:(["title":"the simple novice","xp":1000,"qp":0]),
      3:(["title":"the beginner","xp":1500,"qp":0]),
      4:(["title":"the adventurer","xp":2300,"qp":0]),
      5:(["title":"the experienced adventurer","xp":3500,"qp":5]),
      6:(["title":"the expert adventurer","xp":5100,"qp":12]),
      7:(["title":"the great adventurer","xp":7700,"qp":21]),
      8:(["title":"the master adventurer","xp":12000,"qp":32]),
      9:(["title":"the Freeman","xp":17000,"qp":45]),
      10:(["title":"the Citizen","xp":26000,"qp":60]),
      11:(["title":"the Knight","xp":39000,"qp":77]),
      12:(["title":"the Baron","xp":59000,"qp":96]),
      13:(["title":"the Count","xp":88000,"qp":117]),
      14:(["title":"the Earl","xp":130000,"qp":140]),
      15:(["title":"the Marquis","xp":198000,"qp":165]),
      16:(["title":"the Duke","xp":297000,"qp":192]),
      17:(["title":"the Arch Duke","xp":444444,"qp":221]),
      18:(["title":"Praetor","xp":666667,"qp":252]),
      19:(["title":"Quaestor","xp":1000000,"qp":285]),
      20:(["title":"Caesar","xp":2000000,"qp":450]),
    ]);
}

void init(){
    ::init();
}

int AdvanceDude(mixed arg){
    int level,xp,qp;
    int desired_level,required_xp,required_qp;
    mixed *statlist;
    mapping this_stat;
    int statclass, statlevel, i;

    statlist = this_player()->GetStats();

    if(this_player()->GetKeyName() == "guest"){
        this_object()->eventForce("say I don't promote temporary players.");
        return 1;
    }

    if(!level = this_player()->GetLevel()){
        this_object()->eventForce("say You are confusing me.");
        return 1;
    }

    if(level > 19){
        this_object()->eventForce("say Whoa there, big "
          "stuff. Advancement past level 20 is the "
          "purview of the Trans-Human Elder Guild. "
          "I'm sorry but I can't help you.");
        return 1;
    }

    xp = this_player()->GetExperiencePoints();
    if(!qp = this_player()->GetQuestPoints()) qp = 0;
    desired_level = level+1;
    required_xp = advancement[desired_level]["xp"];
    if(!required_qp = advancement[desired_level]["qp"]) required_qp = 0;

    this_object()->eventForce("say Level "+desired_level+" "
      "with the title of \""+advancement[desired_level]["title"]+"\" "
      "requires "+required_xp+" experience points and "+
      required_qp+" quest points.");

    if( xp > required_xp-1 && qp > required_qp-1){
        this_object()->eventForce("say Congratulations! "
          "You are promoted to level "+desired_level+" and "
          "have earned the name "+this_player()->GetName()+" "
          +advancement[desired_level]["title"]+".");

        this_player()->ChangeLevel(desired_level);
        this_player()->AddTrainingPoints(desired_level);
        this_player()->AddTitle(advancement[desired_level]["title"]);
        this_player()->RemoveTitle(advancement[desired_level-1]["title"]);

        this_player()->save_player((string)this_player()->GetKeyName());

        return 1;
    }

    else this_object()->eventForce("say I'm sorry, "+
          this_player()->GetName()+", but you have not "
          "fulfilled all the requirements of level "+
          desired_level+". Please come back and try "
          "again once you have fulfilled them.");
    return 0;
}

string GetLevelTitle(int level){
    if(!level) level = 1;
    return advancement[level]["title"];
}
