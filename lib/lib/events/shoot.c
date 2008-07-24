static int mustcarry = 1,mustwield = 1;

int MustCarry(int i){
    if(i) mustcarry = i;
    if(mustcarry) return mustcarry;
    else return 0;
}

int MustWield(int i){
    if(i) mustwield = i;
    if(mustwield) return mustwield;
    else return 0;
}

mixed eventShoot(mixed shooter, mixed target){
    string tmp;
    if(target && objectp(target)){
        tmp=target->GetName();
        target = tmp;
    }
    if(target) write("You shoot at "+target+" with your weapon.");

    return 1;
}

mixed CanShoot(object shooter, mixed target){
    object env = environment(this_player());
    if(this_object() == shooter && mustcarry > 0 && environment(this_object()) != this_player()){
        return "#You are not holding the "+remove_article(shooter->GetShort())+".";
    } 
    if(this_object() == shooter && mustwield > 0 && this_object()->GetWorn() == 0 && !creatorp(this_player())){
        return "#You are not wielding the "+remove_article(shooter->GetShort())+".";
    }
    if(env && env->GetProperty("no attack")){
        return "A mystical force prevents your malice.";
    }
    return 1;
}

varargs mixed direct_shoot_obj_at_obj(mixed args...){
    mixed ret;
    ret = CanShoot(args[0],args[1]);
    //tc("%^B_BLACK%^CanShoot("+identify(args[0])+", "+identify(args[1])+"): "+ret);
    return CanShoot(args[0],args[1]);
}

varargs mixed direct_shoot_obj_with_obj(mixed args...){
    return 1;
}

varargs mixed indirect_shoot_obj_with_obj(mixed args...){
    mixed ret;
    ret = CanShoot(args[1],args[0]);
    //tc("%^B_RED%^ CanShoot("+identify(args[1])+", "+identify(args[0])+"): "+ret);
    return CanShoot(args[1],args[0]);
}

varargs mixed indirect_shoot_obj_at_obj(mixed args...){
    return 1;
}

varargs mixed direct_shoot_obj_at_liv(mixed args...){
    return direct_shoot_obj_at_obj(args...);
}

varargs mixed direct_shoot_liv_with_obj(mixed args...){
    return direct_shoot_obj_with_obj(args...);
}

varargs mixed indirect_shoot_liv_with_obj(mixed args...){
    return indirect_shoot_obj_with_obj(args...);
}

varargs mixed indirect_shoot_obj_at_liv(mixed args...){
    return indirect_shoot_obj_at_obj(args...);
}
