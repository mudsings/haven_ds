#include <lib.h>
#include <meal_types.h>

inherit LIB_ITEM;
inherit LIB_DRINK;

private int FlaskUses, FlaskStrength, MaxFlask, EverFill;
private int MealType;
private string FlaskContents;

int direct_fill_obj_with_obj(){ return 1;}
int direct_fill_obj_from_obj(){ return 1;}
int indirect_fill_obj_with_obj(){ return 1;}
int indirect_fill_obj_from_obj(){ return 1;}

int indirect_pour_obj_into_obj(){ return 1;}
int direct_pour_obj_into_obj(){ return 1;}
int indirect_pour_from_obj_into_obj(){ return 1;}
int direct_pour_from_obj_into_obj(){ return 1;}

int direct_pour_obj_out(){ return 1;}
int direct_pour_out_obj(){ return 1;}

int direct_empty_obj(){ return 1;}

mixed indirect_drink_from_obj(){
    if( environment() != this_player() ) return 0;
    if( FlaskUses < 1 ) return "The flask is empty.";
    return 1;
}

static void create(){
    item::create();
    AddSave( ({ "FlaskUses", "FlaskStrength", "MealType", "FlaskContents" }) );
    FlaskUses = 0;
    FlaskStrength = 0;
    EverFill = 0;
    MaxFlask = 5;
    FlaskContents = "empty";
    MealType = MEAL_DRINK;
}

string GetExternalDesc(){
    string ret = "";
    if(!inherits(LIB_BASE_DUMMY,this_object())){
        if(FlaskContents == "empty") ret = "\nIt is empty.";
        else ret = "\nIt contains some "+FlaskContents+".";
    }
    return item::GetExternalDesc()+ret;
}

int CanFillMe(){
    return MaxFlask - FlaskUses;
}

int CanFillOther(){
    if(EverFill) return 99999;
    else return FlaskUses;
}

int SetStrength(int x){ return (FlaskStrength = x); }

mixed GetStrength(){ return FlaskStrength; }

int SetFlaskUses(int x){ return (FlaskUses = x); }

int GetFlaskUses(){ return FlaskUses; }

int SetEverFill(int x){ return (EverFill = x); }

int GetEverFill(){ return EverFill; }

int SetMealType(int x){ return (MealType = x); }

int GetMealType(){ return MealType; }

int SetMaxFlask(int x){ return (MaxFlask = x); }

int GetMaxFlask(){ return MaxFlask; }

string GetFlaskContents(){ return FlaskContents; }

string SetFlaskContents(string str){ 
    SetId(GetId()+({ str }));
    parse_refresh();
    return (FlaskContents = str); 
}

varargs mixed eventEmpty(object who){
    string *tmpid = GetId();
    if(FlaskContents != "empty") tmpid -= ({ FlaskContents });
    if(environment(this_object()) != who){
        write("You aren't holding it.");
        return 1;
    }
    if(!FlaskUses){
        write("The "+GetShort()+" is already empty.");
        say(who->GetName()+" fiddles with "+GetShort()+".");
        return 1;
    }
    if(EverFill){
        write("This cannot be emptied.");
        return 1;
    }
    write("You pour the "+FlaskContents+" out of "+GetShort()+".");
    say(who->GetName()+" pours the "+FlaskContents+" out of "+GetShort()+".");
    SetId(tmpid);
    parse_refresh();
    FlaskContents = "empty";
    SetStrength(0);
    FlaskUses = 0;
    return 1;
}

mixed eventFill(object who, object from){
    int howmuch_me = CanFillMe();
    int howmuch_them = from->CanFillOther();
    if(!from->isDummy() &&
      environment(from) != this_player()){
        write("You aren't holding the "+from->GetKeyName()+".");
        return 1;
    }

    if(from->isDummy() &&
      environment(this_object()) != this_player()){
        write("You aren't holding the "+this_object()->GetKeyName()+".");
        return 1;
    }

    if(from == this_object()){
        write("You can't fill it with itself!");
        return 1;
    }
    if(!howmuch_them){
        write("The "+from->GetKeyName()+" is empty.");
        return 1;
    }
    if(FlaskUses >= MaxFlask){
        FlaskUses = MaxFlask;
        write("The "+this_object()->GetKeyName()+" is already full.");
        return 1;
    }
    if(from->GetFlaskContents() != GetFlaskContents() && GetFlaskContents() != "empty"){
        write("Those are incompatible fluids, and you cannot mix them.");
        return 1;
    }
    if(from->GetStrength() != GetStrength() && GetStrength()){
        write("Those are incompatible fluids, and you cannot mix them.");
        return 1;
    }
    if(howmuch_them < howmuch_me) howmuch_me = howmuch_them;
    FlaskUses += howmuch_me;
    if(!EverFill) from->SetFlaskUses(from->GetFlaskUses() - howmuch_me);
    write("You pour from "+from->GetShort()+" into "+this_object()->GetShort()+".");
    say(who->GetName()+" pours from "+from->GetShort()+
      " into "+this_object()->GetShort()+".");
    FlaskContents = from->GetFlaskContents();
    FlaskStrength = from->GetStrength();
    if(member_array(FlaskContents, GetId()) == -1) 
        SetId(GetId() + ({ FlaskContents }) );
    parse_refresh();
    return 1;
}

varargs mixed eventDrink(object who, object target, string foo){
    write("You drink from "+GetShort()+".");
    say(who->GetName()+" drinks from "+GetShort()+".");
    if(!EverFill) FlaskUses--;
    who->eventDrink(this_object());
    if(!FlaskUses){
        SetId(GetId() - ({ FlaskContents }) );
        parse_refresh();
        FlaskContents = "empty";
        SetStrength(0);
    }
    return 1;
}

mixed CanDrink(object who, string what){
    if(!FlaskUses){
        say(who->GetName()+" sadly looks at "+GetShort()+".");
        return "The "+GetKeyName()+" is empty.";
    }
    return who->CanDrink(this_object());
}

mixed eventPour(object who, object from){
    return eventFill(who, from);
}
