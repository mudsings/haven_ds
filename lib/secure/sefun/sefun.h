#ifndef l_sefun_h
#define l_sefun_h

string absolute_path(string curr, string newpth);
int absolute_value(int x);
varargs string add_article(string str, int def);
int ambassadorp(object ob);
string architecture();
int archp(object ob);
string arrange_string(string str, int x);
string base_name(mixed val);
string cardinal(int x);
varargs string center(string str, int x);
varargs string conjunction(mixed expressions, string coordinator);
string consolidate(int x, string str);
string convert_name(string str);
varargs mixed convert_string(string str, int flag);
mixed copy(mixed val);
varargs int creatorp(object ob);
float currency_inflation(string type);
float currency_rate(string type);
int currency_mass(int x, string type);
int currency_value(int x, string str);
int destruct(object ob);
mixed *distinct_array(mixed *arr);
string domain(mixed val);
int domain_exists(string dmn);
object domain_master(mixed val);
varargs int effective_light(object ob);
varargs void event(string fun, int when, mixed *args, int reg);
mapping expand_keys(mapping mp);
int event_pending(object ob);
int exec(object target, object src);
int file_exists(string str);
string file_privs(string file);
string format_page(string *items, int columns);
varargs object get_object(string str, object player);
varargs mixed get_objects(string str, object player, int no_arr);
int hiddenp(object ob);
int high_mortalp(object ob);
string identify(mixed a);
string item_list(mixed *items);
int leaderp(object ob);
object *livings();
object load_object(string str);
void log_file(string fl, string msg);
int member_group(mixed who, string grp);
int moral_act(object who, object target, int amount);
string *mud_currencies();
string mud_name();
string mudlib();
string mudlib_version();
varargs int newbiep(object ob);
string nominative(mixed val);
string objective(mixed val);
string ordinal(int x);
object parse_objects(object where, string str);
mixed *path_file(mixed full_path);
float percent(mixed numerator, mixed denominator);
varargs int playerp(object ob);
string possessive(mixed val);
string possessive_noun(mixed val);
int query_date(int x);
string query_day_name(int x);
int query_host_port();
int query_hour(int x);
string GetIdle_string(object ob);
int query_minutes(int x);
string query_month(int x);
int query_night();
int query_reset_number();
string query_season(int x);
object query_snoop(object snoopee);
object query_snooping(object ob);
string query_time_of_day();
int query_year(int x);
string reflexive(mixed val);
string remove_article(string str);
string save_file(string str);
varargs void say(mixed str, object ob);
void set_eval_limit(int x);
void set_privs(object ob, string str);
varargs void shout(mixed str, mixed exclude);
varargs void shutdown(int code);
varargs object snoop(object who, object target);
string strip_article(mixed val);
string strip_colours(string str);
void tell_object(object ob, mixed str);
varargs void tell_room(object ob, mixed str, mixed exclude);
varargs object this_agent(mixed val);
function time_event(string tod, function f);
object to_object(mixed target);
varargs int total_light(object ob);
string translate(string str, int prof);
string trim(string str);
mixed unguarded(function f);
object unique(string fn, int rare);
int user_exists(string str);
string user_path(string name);
string version();
varargs int visibility(object ob);
varargs int visible(object detectee_obj, object detector_obj);
varargs string wrap(string str, int width);
void write(string str);
int tail(string file);
void tell_player(mixed player, string msg);
string *GetCustomPath();
string GetCustomCommand(string str);
string dump_socket_status();
string flat_map(mapping mp);
string *get_cmds();
string *get_verbs();
varargs object array get_livings(object ob,int foo);
string local_time(string str);
void reap_dummies();
varargs void reap_other(string str);
mixed reap_list();
string timestamp(string str);
object *get_dupes(object ob, object where);
string get_stack();
mixed hobbled(object ob);
string query_base_currency();
int query_base_rate();
int query_player_money(object who);
int query_base_value(string currency, int amount);
int make_workroom(object dude);
int valid_currency(string str);
varargs int query_invis(object lookee, object looker);
string set_tz(string str);
string admin_email();
int query_night_light(object ob);
int query_day_light(object ob);
int query_ambient_light(object ob);
int reload_room(object ob);
void tc(string str);
string opposite_dir(string str);
mixed *singular_array(mixed *arr);
varargs mixed remove_matching_line(string target, string substring, int i, string exclude);
string truncate(string str, int i);
int check_privs(mixed pretender, mixed target);
int true();
int false();
int indent_file(string filename);
string path_prefix(string str);
varargs string first_string_element(string str, string delimiter, int stripfirst);
string last_string_element(string str, string delimiter);
string replace_line(string file, string *params, string repl);
int memberp(mixed *primary, mixed sub);
int reverse_memberp(string primary, string *sub);
int mgrepp(string primary, string *sub);
int grepp(string primary, string sub);
varargs mixed replace_matching_line(string target, string substring, string replace, int i, string exclude);
varargs mixed remove_matching_line(string target, string substring, int i, string exclude);
string first(string str, int i);
varargs string last(string str, int i, int significant);
int starts_with_arr(string primary, string *sub);
int starts_with(string primary, string sub);
string reverse_string(string str);
mixed homedir(object ob);
varargs string generate_tmp(mixed arg);
string newline_trim(string str);
int directory_exists(string str);
mixed reload(object ob);
string query_os_type();
varargs mixed read_matching_line(string target, string substring, int i, string exclude);
int abs(int x);
int wipe_inv(mixed dude);
int bool_reverse(mixed arg);
mixed check_light(object who);
int query_carrying(object who, mixed thing);

#endif /* l_sefun_h */
