/*    /verbs/players/speak.c
 *    from the Dead Souls Object Library
 *    speak
 *    speak STR
 *    speak in WRD STR
 */



#include <lib.h>
#include <talk_type.h>
#include "include/speak.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("speak");
    SetRules("","in WRD","in WRD STR","STR");
}

mixed can_speak() { return "Speak what? In what language?"; }

mixed can_speak_in_wrd(string str) {
    mixed ret = this_player()->CanSpeak(0, TALK_LOCAL, "foo", str);
    if(intp(ret)) {
        write("You are now speaking in "+capitalize(lower_case(str))+".");
        return 1;
    }
    else return ret;
}

mixed can_speak_str(string str) {
    string lang;

    if( !str ) return 0;
    if( strlen(str) > 3 && str[0..2] == "in " ) return 0;
    lang = (string)this_player()->GetDefaultLanguage() || 
    (string)this_player()->GetNativeLanguage();
    return (mixed)this_player()->CanSpeak(0, TALK_LOCAL, str, lang);
}

mixed can_speak_in_wrd_str(string lang, string str) {
    if( !lang || !str ) return 0;
    if( !environment(this_player()) ) return "You are nowhere right now.";
    return (mixed)this_player()->CanSpeak(0, TALK_LOCAL, str, lang);
}

mixed do_speak() { return 1; }

mixed do_speak_in_wrd(string str) { 
    this_player()->SetDefaultLanguage(str);
    return 1;
}

mixed do_speak_str(string str) {
    string lang;

    lang = (string)this_player()->GetDefaultLanguage() ||
    (string)this_player()->GetNativeLanguage();
    return do_speak_in_wrd_str(lang, str);
}

mixed do_speak_in_wrd_str(string lang, string str) {
    if( str[<1] != '.' && str[<1] != '?' && str[<1] != '!' )
        str = capitalize(str) + ".";
    else str = capitalize(str);
    return (mixed)this_player()->eventSpeak(0, TALK_LOCAL, str, lang);
}

string GetHelp(string str) {
    return ("Syntax: <speak MESSAGE>\n"
      "        <speak in LANGUAGE MESSAGE>\n\n"
      "Sends the message you specify to all people in the same room "
      "as you.  If you are an avatar, you have the ability to customize "
      "the way these messages come out through the \"message\" "
      "command.  If you fail to specify a language, your default "
      "language will be used.\n"
      "To switch your default language to something other than your "
      "native language:\n"
      "speak in FOO\n\n"
      "See also: message, say, shout, speak, tell");
}

