
//	ansi header Creator	:  Gothic@TMI-2  
//
//	The standard set of ANSI codes for mudlib use.
//
// Hi-intensity black added by Blue, 941130.  Why leave it out?

#define ESC	""
#define CSI	ESC + "["

/*  Foreground Colors  */

#define BLK ESC+"[30m"          /* Black    */
#define RED ESC+"[31m"          /* Red      */
#define GRN ESC+"[32m"          /* Green    */
#define YEL ESC+"[33m"          /* Yellow   */
#define BLU ESC+"[34m"          /* Blue     */
#define MAG ESC+"[35m"          /* Magenta  */
#define CYN ESC+"[36m"          /* Cyan     */
#define WHT ESC+"[37m"          /* White    */

/*   Hi Intensity Foreground Colors   */

#define HIK ESC+"[1;30m"        /* Black    */
// Note hi-black comes out as dark grey.
#define HIR ESC+"[1;31m"        /* Red      */
#define HIG ESC+"[1;32m"        /* Green    */
#define HIY ESC+"[1;33m"        /* Yellow   */
#define HIB ESC+"[1;34m"        /* Blue     */
#define HIM ESC+"[1;35m"        /* Magenta  */
#define HIC ESC+"[1;36m"        /* Cyan     */
#define HIW ESC+"[1;37m"        /* White    */

/*  Background Colors  */

#define BBLK ESC+"[40m"          /* Black    */
#define BRED ESC+"[41m"          /* Red      */
#define BGRN ESC+"[42m"          /* Green    */
#define BYEL ESC+"[43m"          /* Yellow   */
#define BBLU ESC+"[44m"          /* Blue     */
#define BMAG ESC+"[45m"          /* Magenta  */
#define BCYN ESC+"[46m"          /* Cyan     */
#define BWHT ESC+"[47m"          /* White    */

/* High Intensity Background Colors  */

#define HBRED ESC+"[41;1m"       /* Red      */
#define HBGRN ESC+"[42;1m"       /* Green    */
#define HBYEL ESC+"[43;1m"       /* Yellow   */
#define HBBLU ESC+"[44;1m"       /* Blue     */
#define HBMAG ESC+"[45;1m"       /* Magenta  */
#define HBCYN ESC+"[46;1m"       /* Cyan     */
#define HBWHT ESC+"[47;1m"       /* White    */

#define NOR ESC+"[2;37;0m"      /* Puts everything back to normal */

/*  Additional ansi Esc codes added to ansi.h by Gothic  april 23,1993 */
/* Note, these are Esc codes for VT100 terminals, and emmulators */
/*       and they may not all work within the mud               */

#define BOLD ESC+"[1m"          /* Turn on bold mode */
#define CLR ESC+"[2J"           /* Clear the screen  */
#define HOME ESC+"[H"           /* Send cursor to home position */
#define REF CLR+HOME            /* Clear screen and home cursor */
#define BIGTOP ESC+"#3"         /* Dbl height characters, top half */
#define BIGBOT ESC+"#4"         /* Dbl height characters, bottem half */
#define SAVEC ESC+"[s"           /* Save cursor position */
#define REST ESC+"[u"            /* Restore cursor to saved position */
#define REVINDEX ESC+"M"        /* Scroll screen in opposite direction */
#define SINGW ESC+"#5"          /* Normal, single-width characters */
#define DBL ESC+"#6"            /* Creates double-width characters */
#define FRTOP ESC+"[2;25r"      /* Freeze top line */
#define FRBOT ESC+"[1;24r"      /* Freeze bottom line */
#define UNFR ESC+"[r"           /* Unfreeze top and bottom lines */
#define BLINK ESC+"[5m"         /* Initialize blink mode */
#define U ESC+"[4m"             /* Initialize underscore mode */
#define REV ESC+"[7m"           /* Turns reverse video mode on */
#define HIREV ESC+"[1,7m"       /* Hi intensity reverse video  */

#define STRIP_ME ({ BLK,RED,GRN,YEL,BLU,MAG,CYN,WHI, \
        HIK, HIR, HIG, HIY, HIB, HIM, HIC, HIW, \
        BBLK, BRED, BGRN, BYEL, BBLU, BMAG, BCYN, BWHI, \
        HBBLK, HBRED, HBGRN, HBYEL, HBBLU, HBMAG, HBCYN, HBWHI, \
        NOR, BOLD, CLR< HOME, REF, BIGTOP, BIGBOT, SAVEC, REST, \
        REVINDEX, SINGW, DBL, FRTOP, FRBOT, UNFR, BLINK, U ESC, REV ESC, HIREV ESC })

#define BEEP    "^G"

/* Program      : /feature/me.c
 * Author       :Kenny@Eternal.Civilization (3/25/1998)
 *
 * port to chinese wizard group : /feature/vi.c by ken@chinesemud.net (99-3-15)
 */


#define ECHO 0                          // �®�ׄ��טӬ��׾��լ
#define HIDE 1                          // �®�ׄ��ט����׾��լ
#define REFRESH 1                       // �®�ׄ����Ӭ�۩���Ȇ
#define LOGFILE "static/VI"             // ����ӌ����í��

void start_edit(string filename);       // ���Ϙ���˕����׭����
private void _append(object pl);        // ������Ȇ����
private void _append_line(object pl);   // ���𘊴��í�ө����
private void _backspace(object pl);     // о����̗����
protected void _confirm_save(string str, object pl); // ��������˕̗���γ���
private void _del_char(object pl, int bs);      // о�����δ���
private void _del_line(object pl);              // о��ؓ��
private int  _dispatcher(object pl);            // ����ٹ����ͼ��׻
private void _down(object pl);                  // ��ͷ����ө��
private void _end(object pl);                   // �����˴����
private void _esc(object pl);                   // ��ׄ����׻�۴�Ȓ��׻
private void _file_end(object pl);              // �����˴����
private void _file_head(object pl);             // �����˴���͸
private void _goto_line(object pl);             // ް���۵��Ϯ�
protected void _goto_line_done(string str, object pl, int rein); // ����Ϧ�
private void _help(object pl);                  // ��ٔ��Ȇ
private void _home(object pl);                  // �����˴�����
protected void _input(string str, object pl, int fresh); // �´�ׄ���ٳ�
private void _insert(object pl);                // ����̗Ȇ����
private void _insert_line(object pl);           // �����θ���ө����
private void _insert_exclam_mark(object pl);    // ����̗Ȇ����ө���!���լ
private void _join(object pl);                  // �����������˴�������Ȇ
private void _keymap(object pl);                // ��Ӆ�à�����
protected void _keymap_done(string str, object pl); // ��ȵ�Ӆ������
private void _left(object pl);                  // ��ͷ���ө��
// ��ڳ޼����׾ҵ��
private varargs void _message(object pl, string str, string callback, int hide,
        mixed arg);
protected void _message_done(string str, object pl); // ��װҵ�����׾
private void _next_match(object pl);            // ��җ/ކ���ù������ٳ�
private void _page_down(object pl);             // ͷ�þ�ө����22 ����
private void _page_up(object pl);               // ͷ�ξ�ө����22 ����
private void _process(object pl);               // ����ׄ����׻���ٳ�ׄ��
protected void _quit(mixed unused, mixed pl);   // ��������˕
private void _refresh(object pl);               // ���é�Ȇ
private void _refresh_cursor(object pl);        // ���������
// �۩���Ȇ
private void _refresh_screen(object pl, int s_row, int e_row, int s_col);
private void _refresh_status(object pl);        // ����ڳ޼��
private void _replace(object pl);               // �ٳ�ކ��
protected void _replace_done(string str, object pl); // ���ކ���ٳ�
private void _right(object pl, int col, int extra);  // ��ͷ����ө��
private void _search(object pl);                     // �ٳ���җ
protected void _search_done(string str, object pl);  // �����җ�ٳ�
private void _tab(object pl);                        // Ȓ��׻�� TAB �������
private void _undef_key(object pl);                  // ��Γ����������Ӆ
protected void _undef_key_done(string str, object pl); // ���ٹ��������
private void _up(object pl);                           // ��ͷ����ө��
private void _write(object pl);                        // ��ȳ󳆴���ȓ��
protected void _write_done(string str, object pl, int quit); // �󳆴���

private mapping EditedFiles=([ ]);              // �����ϴ�����
/*
   int iS_Row, iE_Row,                             // �Э��Գ���׭���Ϧ�
   iS_Col, iE_Col,                             // �Э��Գ���׭����
   iRow,   iCol,                               // �����
   is_Command_Mode, is_NewFile, is_Modify,     // ����ڳ��ˌ��
   is_Substituted;                             // ������Ëˌ��
   string *sText,                                  // ��������
   sChar, sOldChar,                         // ׄ���ٳ���Ȓ��
   sCount,                                  // Ȓ��ײ
   sFileName,                               // ������ȓ
   sSearch, sReplace;                       // ��җ/ކ���ٳ�
   mapping keymap;                                 // ��������Ӆ���ؘ�
 */

void start_edit(string file)                // ���Ϙ���˕����׭����
{
    int  i;
    mapping me;
    //tc("starting edit of "+file);

    if (objectp(EditedFiles[file]))
    {
        write(sprintf("%s ز������؂��������̋�զ��㘀����\n",
                    capitalize(EditedFiles[file]->query("id"))));
        return;
    } // if
    this_player()->SetProperty("me", ([ ]));
    me = this_player()->GetProperty("me");
    me["sFileName"] = file;
    if (file_size(file) <= 0)
    {        // �������팵 1
        me["sText"] = ({ "" });
        if (file_size(file) < 0)
        {        // �ô�
            me["is_Command_Mode"] = 0;
            me["is_NewFile"] = 1;
        }
    }
    else
    {
        string content=read_file(file), tabs="        ";

        // �����񵮳�� 0
        me["is_Command_Mode"] = 1;
        me["is_NewFile"] = 0;
        if (stringp(content))
        {
            me["sText"] = explode(read_file(file), "\n");
            // �� TAB���� 8 ���ؗڳ�ކ���ո�������ȴ���݃
            for (i=sizeof(me["sText"])-1; i>=0; i--)
            {
                int len=sizeof(me["sText"][i]);
                reset_eval_cost();
                for (int j=0; j<len; j++)
                    if (me["sText"][i][j] == '\t')
                        me["sText"][i] = replace_string(
                                me["sText"][i], "\t", tabs[0..7-j%8]);
            }
        }
        else me["sText"] = ({ "" });
    } // old file

    me["sCount"] = "";
    me["sSearch"] = me["sReplace"] = me["is_Modify"] = 0;
    me["iE_Row"] = 23;
    me["iS_Row"] = me["iS_Col"] = me["iRow"] = me["iCol"] = 1;
    EditedFiles += ([ file: this_player()->link() ]);

    // ���/Ј����������Ӆ���ؘ�
    if (!mapp(me["keymap"] = this_player()->query("me_keymap")))
        me["keymap"] = ([ ]);
    if (this_player()->query("me_no_help") ||
            this_player()->GetProperty("no_me_help"))
    {        // �̴�ө��נ�Ș���˕��٘�Ի�������˕
        _refresh_screen(this_player(), 1, 23, 1);

        get_char("_input", HIDE, this_player());
    }
    else
    {        // ��ө��נ�Ș���˕��ή������ݴȕ
        this_player()->SetProperty("no_me_help", 1);
        _help(this_player());
    }
} // start_edit()


private void _refresh_cursor(object pl)        // ���������
{
    mapping me=pl->GetProperty("me");
    printf(ESC "[%d;%df", me["iRow"]+1, me["iCol"]);
} // _refresh_cursor()


private void _refresh_status(object pl)        // ����ڳ޼��
{
    mapping me=pl->GetProperty("me");

    // �˵��𘊴� (1,1)���ȸ�΀��լ��׾ڳ޼��
    printf(HOME + NOR + REV + "��:%4d ��:%3d %-43s %10s [Ȓ��׻]" + NOR,
            me["iS_Row"]+me["iRow"]-1, me["iS_Col"]+me["iCol"]-1,
            sprintf("%s %s", me["sFileName"], (me["is_Modify"]? "(��ϝ��)":
                    (me["is_NewFile"]? "(�ô�)": "")))[0..42],
            (me["is_Command_Mode"]? "?:��ٔ��Ȇ": "[ׄ����׻]"));
    _refresh_cursor(pl);
} // _refresh_status()


// �۩���Ȇ
private void _refresh_screen(object pl, int s_row, int e_row, int s_col)
{
    int  i, temp;
    mapping me=pl->GetProperty("me");


    if (e_row < s_row)                // ����׭���Ϧʴ�ή�߳���
    {
        temp = e_row;
        e_row = s_row;
        s_row = temp;
    }

    if (s_row < 1) s_row = 1;
    if (s_col < 1) s_col = 1;
    if (e_row > sizeof(me["sText"])) // �����������
        e_row = sizeof(me["sText"]);
    if (e_row-s_row > 22) e_row = s_row+22;

    me["iS_Row"] = s_row;        me["iE_Row"] = e_row;
    me["iS_Col"] = s_col;        me["iE_Col"] = s_col+79;

    printf(REF+NOR);                // ̅����Ȇ���˵��𘊴� (1,1)
    for (i=me["iS_Row"]-1; i<me["iE_Row"]; i++)
        printf("\r\n%s", me["sText"][i][me["iS_Col"]-1..me["iE_Col"]-1]);
    for (i=me["iS_Row"]+22-me["iE_Row"]; i>0; i--)
        printf("\r\n~");        // ��ڃ�Ϧ��� ~ ��׾
    _refresh_status(pl);
} // _refresh_screen()


protected void _input(string str, object pl, int fresh) // �´�ׄ���ٳ�
{
    int    i, size, bs=0;
    string *chars;
    mapping me=pl->GetProperty("me");
    //tc("_input("+identify(str)+", "+identify(pl)+", "+identify(fresh)+")");
    if (fresh)
    {        // �۩���Ȇ
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
        get_char("_input", HIDE, pl);
        return;
    }
    //tc("1");
    me["is_Substituted"] = 1;       // ήЈ��������Ëˌ��
    if (str == "")                  // ������ Backspace/Ctrl-Backspace
    {
        str = "BACKSPACE";
        bs = 1;
    }
    if (str[0] == 27)
    {
        chars = explode(str, ESC);      // Ӝ��Ë���ٳ����� ESC ����
        for (i=sizeof(chars)-1; i>=0; i--)
            chars[i] = ESC + chars[i];      // �ԩ� ESC
        if (sizeof(chars) == 0)                 // ��٩�̴�ө��լ ESC
            chars = ({ ESC });
    }
    else    chars = ({ str });

    size = sizeof(chars);
    for (i=0; i<size; i++)
    {        // ڬ��Ӝ��Ë𬘀��˕��Ë
        switch (chars[i])
        {
            case "\t":       me["sChar"] = "TAB";   break;
            case "\r":       me["sChar"] = "ENTER"; break;
            case ESC:        me["sChar"] = "ESC";   break;
                             case ESC "OA":
                                 case ESC "[A":   me["sChar"] = "UP";    break;
                             case ESC "OB":
                                 case ESC "[B":   me["sChar"] = "DOWN";  break;
                             case ESC "OC":
                                 case ESC "[C":   me["sChar"] = "RIGHT"; break;
                             case ESC "OD":
                                 case ESC "[D":   me["sChar"] = "LEFT";  break;
                             case ESC "OH":
                                 case ESC "[H":
                                 case ESC "1~":
                                 case ESC "[1~":  me["sChar"] = "HOME";  break;
                             case ESC "[2~":  me["sChar"] = "INSERT";break;
                             case ESC "[3~":  me["sChar"] = "DELETE";break;
                             case ESC "4~":   me["sChar"] = "END";   break;
                             case ESC "[4~":  me["sChar"] = "END";   break;
                             case ESC "[5~":  me["sChar"] = "PAGEUP";break;
                             case ESC "[6~":  me["sChar"] = "PAGEDOWN";break;
                             case ESC "OP":   me["sChar"] = "F1";    break;
                             case ESC "OQ":   me["sChar"] = "F2";    break;
                             case ESC "OR":   me["sChar"] = "F3";    break;
                             case ESC "OS":   me["sChar"] = "F4";    break;
                             case ESC "m":    me["sChar"] = "F5";    break;
                             case ESC "[17~": me["sChar"] = "F6";    break;
                             case ESC "[18~": me["sChar"] = "F7";    break;
                             case ESC "[19~": me["sChar"] = "F8";    break;
                             case ESC "[20~": me["sChar"] = "F9";    break;
                             case ESC "[21~": me["sChar"] = "F10";   break;
                             case ESC "[23~": me["sChar"] = "F11";   break;
                             case ESC "[24~": me["sChar"] = "F12";   break;
            default:
                             if (sizeof(chars[i])==1 &&
                                     0<chars[i][0] && chars[i][0]<27)
                                 switch (chars[i][0])
                                 { // ������Ë��נ��ׄ����׻ә��נ��ٹ
                                     case  1: me["sChar"]="Ctrl-A"; break;
                                     case  2: me["sChar"]="Ctrl-B"; break;
                                     case  4: me["sChar"]="Ctrl-D"; break;
                                     case  6: me["sChar"]="Ctrl-F"; break;
                                     case  7: me["sChar"]="Ctrl-G"; break;
                                     case 10: me["sChar"]="Ctrl-J"; break;
                                     case 11: me["sChar"]="Ctrl-K"; break;
                                     case 12: me["sChar"]="Ctrl-L"; break;
                                     case 14: me["sChar"]="Ctrl-N"; break;
                                     case 15: me["sChar"]="Ctrl-O"; break;
                                     case 16: me["sChar"]="Ctrl-P"; break;
                                     case 17: me["sChar"]="Ctrl-Q"; break;
                                     case 18: me["sChar"]="Ctrl-R"; break;
                                     case 19: me["sChar"]="Ctrl-S"; break;
                                     case 20: me["sChar"]="Ctrl-T"; break;
                                     case 21: me["sChar"]="Ctrl-U"; break;
                                     case 22: me["sChar"]="Ctrl-V"; break;
                                     case 23: me["sChar"]="Ctrl-W"; break;
                                     case 24: me["sChar"]="Ctrl-X"; break;
                                     case 25: me["sChar"]="Ctrl-Y"; break;
                                     case 26: me["sChar"]="Ctrl-Z"; break;
                                 } // switch
                             else
                             {
                                 me["sChar"] = chars[i];            // �����Ϳ�ٳ�
                                 if (!bs) me["is_Substituted"] = 0; // ̅��������
                             } // else
        } // switch
        // ��ڬ���ߴ��ٳ��ʹ�����ٹ��׻�������󴮳��®��ù�ׄ��
        //tc("me[\"sChar\"]: "+me["sChar"], "blue");
        if (_dispatcher(pl) && i==size-1) get_char("_input", HIDE, pl);
    } // for
} // input()


private int _dispatcher(object pl)                // ����ٹ����ͼ��׻
{
    int  i, count=1;
    mapping me=pl->GetProperty("me");


    if ( me["is_Command_Mode"] ||
            (me["is_Substituted"] && // ��ׄ����׻ә��נ����Ϩٹ
             // ؂����ٹ����؂����
             me["sChar"]!="ENTER" && me["sChar"]!="TAB") )
    {        // ������Ȓ��׻
        if (sizeof(me["sChar"])!=1 && !me["is_Substituted"])
            return 1;
        // ������������Ӆ���ؘ������õ�ӅȒ
        if (!undefinedp(me["keymap"][me["sChar"]]))
            me["sChar"] =
                me["keymap"][me["sChar"]];
        if (!stringp(me["sChar"])) return 1; // Type checking...
        // ������ײ�ٴ��������̹��Դ�Ȓ��ײ��
        if ('0'<=me["sChar"][0] && me["sChar"][0]<='9')
            me["sCount"] += me["sChar"];
        else
        {
            sscanf(me["sCount"], "%d", count);
            me["sCount"] = ""; // ��ײ˕��
            // �۹��ι�ٹ
            if (me["sChar"] == ".")
                me["sChar"] = me["sOldChar"];
            else        // ��í؂���ٹ
                me["sOldChar"] = me["sChar"];
        }

        for (i=0; i<count; i++)
            switch (me["sChar"]) // ����ٹ����ͼ��׻
            {
                case "a":_append(pl);                           break;
                case "d":
                    case "Ctrl-D": _del_line(pl);                  break;
                case "f":
                    case "Ctrl-Z": _refresh(pl);                break;
                case "g":
                    case "Ctrl-G": _goto_line(pl);                break;
                case "h":
                    case "LEFT":
                    case "Ctrl-J": _left(pl);                   break;
                case "i":      _insert(pl);                break;
                case "j":
                    case "DOWN":
                    case "Ctrl-K": _down(pl);                break;
                case "k":
                    case "UP":
                    case "Ctrl-O": _up(pl);                        break;
                case "l":
                    case "RIGHT":
                    case "Ctrl-L": _right(pl, 1,
                            (me["is_Command_Mode"]?0:1));
                break;
                case "m": _keymap(pl);                        break;
                case "n":
                    case "Ctrl-N": _next_match(pl);                break;
                case "o": _append_line(pl);                    break;
                case "q":        // ��׾����®���ׄ������װ����
                    case "F4":
                    case "Ctrl-Q": _quit(0, pl);                return 0;
                case "r":
                    case "Ctrl-R": _replace(pl);                break;
                case "s":
                    case "Ctrl-S": _search(pl);                   break;
                case "u": _undef_key(pl);                break;
                case "w":
                    case "F2":
                    case "Ctrl-W": _write(pl);                      break;
                case "x":
                    case "DELETE":
                    case "Ctrl-X":   _del_char(pl, 0);        break;
                case "A":
                    case "HOME":
                    case "Ctrl-A":   _home(pl);        break;
                case "B":
                    case "PAGEUP":
                    case "Ctrl-B":   _page_up(pl);        break;
                case "E":
                    case "END":
                    case "Ctrl-E":   _end(pl);        break;
                case "F":
                    case "PAGEDOWN":
                    case "Ctrl-F":   _page_down(pl);        break;
                case "J":
                    case "Ctrl-P": _join(pl);         break;
                case "O": _insert_line(pl);         break;
                case "U":
                    case "Ctrl-U": _file_head(pl);        break;
                case "V":
                    case "Ctrl-V": _file_end(pl);        break;
                case "?":
                    case "F1":
                    case "Ctrl-Y": _help(pl);         break;
                case "~":
                    case "Ctrl-T": _insert_exclam_mark(pl);        break;
                case "BACKSPACE": _backspace(pl);          break;
                case "ESC": _esc(pl);                         break;
                case "TAB": _tab(pl);                        break;
            } // switch
    } // if me["is_Command_Mode"]
    else
        _process(pl);                // ����ׄ����׻���ٳ�ׄ��

    return 1;                        // ��׾��ϰ�®���ׄ������ϰ����
} // dispatcher()


private void _append(object pl)                // ������Ȇ����
{
    mapping me=pl->GetProperty("me");

    if (sizeof(me["sText"]) == 0) return;
    me["is_Command_Mode"] = 0;
    _refresh_status(pl);
    _right(pl, 1, 1);
} // _append()


private void _append_line(object pl)        // ���𘊴��í�ө����
{
    string *text;
    mapping me=pl->GetProperty("me");

    me["is_Command_Mode"] = me["is_NewFile"] = 0;
    me["is_Modify"] = 1;
    text = me["sText"][0..me["iS_Row"]+me["iRow"]-2] + ({ "" }) +
        me["sText"][me["iS_Row"]+me["iRow"]-1..<1];
    me["sText"] = text;
    _refresh_screen(pl, me["iS_Row"], sizeof(me["sText"]), 1);
    me["iCol"] = 1;
    _down(pl);
} // _append_line()


private void _backspace(object pl)        // о����̗����
{
    mapping me=pl->GetProperty("me");

    if (me["iS_Col"] == me["iCol"])        return;
    _left(pl);
    _del_char(pl, 1);
} // _backspace()


protected void _confirm_save(string str, object pl) // ��������˕̗���γ���
{
    mapping me=pl->GetProperty("me");

    if (str=="y" || str=="Y") return _write_done("", pl, 1);
    me["is_Modify"] = 0;                // Ј����ϝ���ˌ��
    _quit(0, pl);
} // _confirm_save()


private void _del_char(object pl, int bs)                // о�����δ���
{
    int    go_left=0;                // �̸���о����Ӭ�����
    string str;
    mapping me=pl->GetProperty("me");


    if (sizeof(me["sText"])==0 ||
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])==0)
        return;
    if (me["is_NewFile"] || !me["is_Modify"])
    {
        me["is_NewFile"] = 0;
        me["is_Modify"] = 1;
        _refresh_status(pl);
    }
    if (me["iS_Col"]+me["iCol"]-2 >=
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        return;
    str = me["sText"][me["iS_Row"]+me["iRow"]-2][0..me["iS_Col"]+
        me["iCol"]-3];
    // �����̸�ψӬ�������ô�����
    if (me["iS_Col"]+me["iCol"]-1 <
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        str += me["sText"][me["iS_Row"]+
            me["iRow"]-2][me["iS_Col"]+me["iCol"]-1..<1];
    else        go_left = 1;                // �����о����������ݷ��Ӭ� 
    me["sText"][me["iS_Row"]+me["iRow"]-2] = str;
    printf(ESC "[%d;1f%-80s", me["iRow"]+1,        me["sText"][me["iS_Row"]+
            me["iRow"]-2][me["iS_Col"]-1..me["iE_Col"]-1]);
    if (go_left && !bs) _left(pl);
    else        _refresh_cursor(pl);
} // _del_char()


private void _del_line(object pl)        // о��ؓ��
{
    string *text=({ });
    mapping me=pl->GetProperty("me");
    me["is_NewFile"] = 0;
    me["is_Modify"] = 1;
    if (me["iS_Row"]+me["iRow"]-2 > 0)
        text = me["sText"][0..me["iS_Row"]+me["iRow"]-3];
    text += me["sText"][me["iS_Row"]+me["iRow"]-1..<1];
    me["sText"] = text;
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
    if (me["iS_Row"]+me["iRow"]-1 > sizeof(me["sText"]))
        _up(pl);                // ����о��ڎ��ө������Ӭ������
    // ���𘊴������������������˵��𘊴����
    else if (me["iS_Col"]+me["iCol"]-1 >
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        _end(pl);
} // _del_line()


private void _down(object pl)                // ��ͷ����ө��
{
    int  acc;
    mapping me=pl->GetProperty("me");


    // �����̸��������
    if (me["iS_Row"]+me["iRow"]-1 >= sizeof(me["sText"]))
        // ������׾�����ө������������
        if (me["iRow"] < 23) return;
        else
        {        // �����Դ����ө������ͩ�մ�������
            me["iS_Row"]++;
            printf(ESC "[24;1f\r\n~");
            me["iRow"] = 22;
        }
    else if (++me["iRow"] > 23)
    {        // ��Ȇ�ξ�����׾��ө��
        me["iS_Row"]++;                me["iE_Row"]++;
        printf(ESC "[24;1f\r\n%s", me["sText"][me["iE_Row"]-1]
                [me["iS_Col"]-1..me["iE_Col"]-1]);
        me["iRow"] = 23;
    }

    // ���𘊴������������������˵��𘊴����
    if (me["iS_Col"]+me["iCol"]-1 >
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        me["iCol"] = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
            me["iS_Col"]+(me["is_Command_Mode"]?1:2);
    // ��������������������ө��
    if (me["iCol"]==0 && me["iS_Col"]==1) me["iCol"] = 1;
    if (me["iCol"] > 0)_refresh_status(pl);
    else
    {        // �𘊴��������ա��Ȇ͂��ψӬ�۩���Ȇ
        acc = me["iCol"];        me["iCol"] = 1;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"],
                me["iS_Col"]+acc-1);
    }
} // _down()


private void _end(object pl)                // �����˴����
{
    mapping me=pl->GetProperty("me");

    if (sizeof(me["sText"]) == 0) return;
    // ��݃�������
    me["iCol"] = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
        me["iS_Col"]+(me["is_Command_Mode"]?1:2);
    if (me["iCol"] == 0) me["iCol"] = 1; // ��������������������ө��
    if (sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]) < me["iE_Col"])
        _refresh_status(pl);
    else
    {        // �𘊴��������ա��Ȇ͂��ψӬ�۩���Ȇ
        me["iCol"] = 79;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]+
                (sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
                 me["iE_Col"]+1));
    }
} // _end()


private void _esc(object pl)                // ��ׄ����׻�۴�Ȓ��׻
{
    mapping me=pl->GetProperty("me");
    me["is_Command_Mode"] = 1;
    _left(pl);
} // _esc()


private void _file_end(object pl)        // �����˴����
{
    mapping me=pl->GetProperty("me");
    if (sizeof(me["sText"]) == 0) return;
    _goto_line_done(sprintf("%d", sizeof(me["sText"])), pl, 0);
} // _file_end()


private void _file_head(object pl)        // �����˴���͸
{
    mapping me=pl->GetProperty("me");
    if (sizeof(me["sText"]) == 0) return;
    _goto_line_done("1", pl, 0);
} // _file_head()


private void _goto_line(object pl)        // ް���۵��Ϯ�
{
    mapping me=pl->GetProperty("me");
    _message(pl, sprintf("̋ׄ���Ϧ���٘�ԗ� ENTER ��Γ��1~%d)��",
            sizeof(me["sText"])), "_goto_line_done", ECHO, 1);
} // _goto_line()


protected void _goto_line_done(string str, object pl, int rein) // ����Ϧ�
{
    int        no;
    mapping me=pl->GetProperty("me");

    if (str == "") return _message_done("", pl);
    if (sscanf(str, "%d", no)!=1 || no<1 || no>sizeof(me["sText"]))
        return _message(pl, "������Ϧ���", "_message_done", HIDE);

    me["iCol"] = 1;
    if (me["iS_Row"]<=no && no<=me["iE_Row"])
    {        // ٹ���Ϧ���ա���Ը����
        me["iRow"] = no-me["iS_Row"]+1;
        _refresh_status(pl);
    }
    else
    {        // ٹ���Ϧʙ���ա���Ը������ψӬ�۩���Ȇ
        me["iS_Row"] = no-11;
        if (me["iS_Row"] < 1)        me["iS_Row"] = 1;
        if (me["iE_Row"] < no+11)        me["iE_Row"] = no+11;
        me["iRow"] = no-me["iS_Row"]+1;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], 1);
    }
    if (rein) get_char("_input", HIDE, pl); // �����̸�ψӬ�®��ó�ׄ��
} // _goto_line_done()


private void _help(object pl)                // ��ٔ��Ȇ
{
    printf(REF +
            "� LPMud ��Ԩ������˕ ��1.3 �� 3/25/1998  by ϩ��Ή(Kenny Hsieh), ����ݷ��.\r
            \r
            a            ������Ȇ����               d  Ctrl-D    о��ؓ��\r
            f  Ctrl-Z    ���é�Ȇ                     g  Ctrl-G    ް���۵��Ϯ�\r
            h  Ctrl-J    ��ͷ���ө��               i            ����̗Ȇ����\r
            j  Ctrl-K    ��ͷ����ө��               k  Ctrl-O    ��ͷ����ө��\r
            l  Ctrl-L    ��ͷ����ө��               m            ��Ӆ�à�\����\r
            n  Ctrl-N    ��җ/ކ���ù������ٳ�        o            ���𘊴��í�ө����\r
            q  Ctrl-Q    ��������˕                   r  Ctrl-R    �ٳ�ކ��\r
            s  Ctrl-S    �ٳ���җ                     u            ��Γ����\������Ӆ\r
            w  Ctrl-W    �󳆴���                     x  Ctrl-X    о�����δ���\r
            A  Ctrl-A    �����˴�����               B  Ctrl-B    ͷ�ξ�ө����22 ����\r
            E  Ctrl-E    �����˴����               F  Ctrl-F    ͷ�þ�ө����22 ����\r
            J  Ctrl-P    ���������ٻԴ�������Ȇ       O            �����θ���ө����\r
            U  Ctrl-U    �����˴���͸               V  Ctrl-V    �����˴����\r\n");
    printf(
            ".            �۹��ι�ٹ                 ?  Ctrl-Y    ��ٔ��Ȇ���ݩ�Ȇ��\r
            ~  Ctrl-T    ����̗Ȇ����ө���!���լ\r\n
            �θ�����Ӆ�������������� ESC��F1�F12��BACKSPACE��TAB��ENTER��INSERT��DELETE��\r
            HOME��END��PAGEUP��PAGEDOWN��UP��DOWN��LEFT��RIGHT��Ctrl-A�Ctrl-Z��\r
            \r
            ��ԡ���ѳݘ���˕����̋������ϓ�����Ϯ����ނ�ͻ�Ӊ̋¬Ç����\r
            kenny@cindy.cis.nctu.edu.tw��\r
            [����ӂ���۴�����˕]"
          );
    get_char("_input", HIDE, pl, REFRESH); // �®헳�����۩���Ȇ
} // _help()


private void _home(object pl)                // �����˴�����
{
    mapping me=pl->GetProperty("me");

    me["iCol"] = 1;
    if (me["iS_Col"] == 1)_refresh_status(pl);
    else        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], 1);
} // _home()


private void _insert(object pl)                // ����̗Ȇ����
{
    mapping me=pl->GetProperty("me");

    if (sizeof(me["sText"]) == 0) return;
    me["is_Command_Mode"] = 0;
    _refresh_status(pl);
} // _insert()


private void _insert_line(object pl)        // �����θ���ө����
{
    string *text=({ });
    mapping me=pl->GetProperty("me");

    me["is_Command_Mode"] = me["is_NewFile"] = 0;
    me["is_Modify"] = 1;
    // �����̸����θ��̸�������
    if (me["iS_Row"]+me["iRow"]-3 >= 0)
        text = me["sText"][0..me["iS_Row"]+me["iRow"]-3];
    text += ({ "" });
    text += me["sText"][me["iS_Row"]+me["iRow"]-2..<1];
    me["sText"] = text;
    me["iCol"] = 1;
    _refresh_screen(pl, me["iS_Row"], sizeof(me["sText"]), 1);
} // _insert_line()


private void _insert_exclam_mark(object pl) // ����̗Ȇ����ө���!���լ
{
    mapping me=pl->GetProperty("me");

    // ���𝸿ز��ׄ���!���ݷ���۴ۭԳݦ�׻��ׄ���!���
    if (sizeof(me["sText"]) == 0) return;
    me["sChar"] = "!";
    _process(pl);
} // _insert_!()


private void _join(object pl)                // �����������˴�������Ȇ
{
    string *text=({ });
    mapping me=pl->GetProperty("me");


    if (me["iS_Row"]+me["iRow"]-1 >= sizeof(me["sText"])) return;

    me["is_NewFile"] = 0;
    me["is_Modify"] = 1;
    _end(pl);                        // ή�����˴����
    text = me["sText"][0..me["iS_Row"]+me["iRow"]-2];
    if (sizeof(me["sText"][me["iS_Row"]+me["iRow"]-1]))
    {        // ��������������������
        text[me["iS_Row"]+me["iRow"]-2] += (" "+
                me["sText"][me["iS_Row"]+me["iRow"]-1]);
        _right(pl, 1, 1);
    }
    text += me["sText"][me["iS_Row"]+me["iRow"]..<1];
    me["sText"] = text;
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
} // _join()


private void _keymap(object pl)                // ��Ӆ�à�����
{
    _message(pl, "̋ׄ���õ�Ӆ����ա��\������٘�ԗ� ENTER ��Γ�����ؗڹ�����",
            "_keymap_done", ECHO);
} // _keymap()


protected void _keymap_done(string str, object pl) // ��ȵ�Ӆ������
{
    string old, _new;
    mapping me=pl->GetProperty("me");

    _refresh_status(pl);
    if (str != "")
    {        // ��������ׄ��
        if (sscanf(str, "%s %s", _new, old) != 2)
            return _message(pl, "ׄ����׻�����", "_message_done",
                    HIDE);
        me["keymap"][_new] = old;
        _message(pl, sprintf("�������Գ������õ�Ӆ���%s����ա��\����?
                    ?s����\������",
                    _new, old), "_message_done", HIDE);
    }
    else        get_char("_input", HIDE, pl); // ��Γׄ������ϰ�®��ù�ׄ��
} // _keymap_done()


private void _left(object pl)                // ��ͷ���ө��
{
    mapping me=pl->GetProperty("me");

    if (me["iS_Col"]==1 && me["iCol"]==1) return _refresh_status(pl);
    if (--me["iCol"] < 1)
    {        // �𘊴��������ա��Ȇ͂��ψӬ�۩���Ȇ
        // ����� 8 ��޽����������٩���ө��
        if (me["iS_Col"]-8 < 1)
            me["iCol"] = me["iS_Col"]-1;
        else        me["iCol"] = 8; // ������Ј�������� 8 �����
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"],
                me["iS_Col"]-me["iCol"]);
        return;
    }
    _refresh_status(pl);
} // _left()


// ��ڳ޼����׾ҵ��
private varargs void _message(object pl, string str, string callback, int hide,
        mixed arg)
{
    printf(HOME + NOR + REV);        // �˵��𘊴� (1,1)��Ј����΀��լ��׾
    // ̅��աڳ޼�ϴ�ҵ��
    printf("
            ");
    printf(HOME + str);                // �˵��𘊴� (1,1)������׾ҵ��
    input_to(callback, hide, pl, arg); // �®�ׄ������
} // _message()


protected void _message_done(string str, object pl) // ��װҵ�����׾
{
    _refresh_status(pl);
    get_char("_input", HIDE, pl);        // ��ϰ�®�ׄ���ٳ�
} // _message_done()


private void _next_match(object pl)                // ��җ/ކ���ù������ٳ�
{
    mapping me=pl->GetProperty("me");
    string str="";
    // Ј���׭��җ�������������
    int    j=me["iS_Col"]+me["iCol"]-1,
           i, ofs;


    if (!stringp(me["sSearch"])) return; // ��ȩήЈ����җ�ٳ���������

    for (i=me["iS_Row"]+me["iRow"]-2; i<sizeof(me["sText"]); i++)
    {
        if ((ofs = strsrch(me["sText"][i][j-1..<1], me["sSearch"]))== -1)
            j = 1; // ؂��ȩ�Ӵ�������Ј�����ϳԴ�ө����׭��
        else
        {        // �Ӵ�ٹ���ٳ���, �˵��𘊴�������
            _goto_line_done(sprintf("%d", i+1), pl, 0);
            me["iCol"] = j+ofs; // ��݃ٹ���ٳ����������
            // ��ٹ���ٳ�Ä��ա��Ȇ͂�����۩���Ȇ
            if (me["iCol"]<me["iS_Col"] ||
                    me["iCol"]+strlen(me["sSearch"])>me["iS_Col"]+79)
                _refresh_screen(pl, me["iS_Row"], me["iE_Row"],
                        me["iCol"]);
            if (stringp(me["sReplace"]))
            {        // ψӬކ���ٳ�
                me["is_NewFile"] = 0;
                me["is_Modify"] = 1;
                // ������̗����
                str = me["sText"][i][0..me["iCol"]-2];
                str += me["sReplace"];
                str += (me["sText"][i][me["iCol"]+
                        strlen(me["sSearch"])-1..<1]);
                me["sText"][i] = str;
                // ��ކ���ٳ�Ä��ա��Ȇ͂�����۩���Ȇ
                if (me["iCol"]<me["iS_Col"] ||
                        me["iCol"]+strlen(me["sReplace"])
                        > me["iS_Col"]+79)
                    _refresh_screen(pl, me["iS_Row"],
                            me["iE_Row"], me["iCol"]);
                // ����˯�˴�ކ���ٳ���Ȇ
                me["iCol"] = me["iCol"]+strlen(me["sReplace"])-
                    me["iS_Col"]+1;
                printf(ESC "[%d;1f%-80s", me["iRow"]+1,
                        me["sText"][me["iS_Row"]+
                        me["iRow"]-2][me["iS_Col"]-1
                        ..me["iE_Col"]-1]);
            }
            // ����˯�˴���җ�ٳ���Ȇ
            else        
                me["iCol"] = me["iCol"]+strlen(me["sSearch"])-me["iS_Col"]+1;
            _refresh_status(pl);
            break;
        } // if found
    } // for each row
    if (i >= sizeof(me["sText"]))
        _message(pl, "�ә���ٹ���ٳ���", "_message_done", HIDE);
    else        get_char("_input", HIDE, pl); // �®��ù�ׄ���ٳ�
} // _next_match()


private void _page_down(object pl)        // ͷ�þ�ө����22 ����
{
    mapping me=pl->GetProperty("me");

    if (me["iS_Row"]+22 > sizeof(me["sText"])) return; // �����̸������㾍��
    me["iS_Row"] += 22;        me["iE_Row"] += 22;
    // �����𘊴�������̸��������
    if (me["iS_Row"]+me["iRow"]-1 > sizeof(me["sText"]))
        me["iRow"] = sizeof(me["sText"])-me["iS_Row"]+1;
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
    // ���𘊴������������������˵��𘊴����
    if (me["iS_Col"]+me["iCol"]-1 > sizeof(me["sText"][me["iS_Row"]+
                me["iRow"]-2]))
        _end(pl);
} // _page_down()


private void _page_up(object pl)        // ͷ�ξ�ө����22 ����
{
    mapping me=pl->GetProperty("me");

    if (me["iS_Row"] <= 1) return; // �����̸������㾍��
    me["iS_Row"] -= 22;
    if (me["iS_Row"] < 1) me["iS_Row"] = 1; // ڎ��٩��������ө��
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
    // ���𘊴������������������˵��𘊴����
    if (me["iS_Col"]+me["iCol"]-1 >
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2]))
        _end(pl);
} // _page_up()


private void _process(object pl)        // ����ׄ����׻���ٳ�ׄ��
{
    int    i, size;
    string str, rest, *input;
    mapping me=pl->GetProperty("me");


    if (me["is_Modify"]==0 || me["is_NewFile"]==1)
    {
        me["is_NewFile"] = 0;
        me["is_Modify"] = 1;
        _refresh_status(pl);
    }
    if (!me["is_Substituted"] && (strsrch(me["sChar"], "\r")!=-1 ||
                strsrch(me["sChar"], "\n")!=-1))
        me["is_Substituted"] = 1; // Ј����Ë����ˌ��
    me["sChar"] = replace_string(me["sChar"], "\t", "        ");
    me["sChar"] = replace_string(me["sChar"], "\r", "\rENTER\r");
    me["sChar"] = replace_string(me["sChar"], "\n", "\rENTER\r");
    input = explode(me["sChar"], "\r");

    size = sizeof(input);
    for (i=0; i<size; i++)                // ө����ө��ׄ��
    {
        if (me["is_Substituted"] && input[i] == "ENTER")
        {        // ������ ENTER ט��Ӭ����٫�ߴ������˴���Ȇ������
            if(!me || !me["sText"] || !me["iS_Row"]){
                continue;
            }
            rest = me["sText"][me["iS_Row"]+
                me["iRow"]-2][me["iS_Col"]+me["iCol"]-2..<1];
            me["sText"][me["iS_Row"]+me["iRow"]-2] =
                me["sText"][me["iS_Row"]+
                me["iRow"]-2][0..me["iS_Col"]+me["iCol"]-3];
            _append_line(pl); // ���𘊴��í�ө����
            // ���ϴ�������ա��������ߴ�����
            me["sText"][me["iS_Row"]+me["iRow"]-2] = rest;
            continue;
        }

        // ׄ���ٳ����� ENTER ט, ���� TAB �����
        if (me["is_Substituted"] && input[i] == "TAB")
            input[i] = "        "[0..7-(me["iCol"]-1)%8];
        str = me["sText"][me["iS_Row"]+me["iRow"]-2][0..me["iS_Col"]+
            me["iCol"]-3] + input[i] + me["sText"][me["iS_Row"] +
            me["iRow"]-2][me["iS_Col"]+ me["iCol"]-2..<1];
        me["sText"][me["iS_Row"]+me["iRow"]-2] = str;
        _right(pl, sizeof(input[i]), 1);
    } // for

    printf(ESC "[%d;1f%s", me["iRow"]+1, me["sText"][me["iS_Row"]+
            me["iRow"]-2][me["iS_Col"]-1..me["iE_Col"]-1]);
    _refresh_cursor(pl);
} // _process()


protected void _quit(mixed unused, mixed pl) // ��������˕
{
    mapping me;

    seteuid(geteuid(pl));
    me = pl->GetProperty("me");
    if (me["is_Modify"])
        return _message(pl, "�ݴ����ҹ�����̋��ψӬ�������y/n)��[n]",
               "_confirm_save", ECHO);
    me["iRow"] = 23;
    _refresh_cursor(pl);                // �˵��𘊴�ڎ��ө��
    printf(NOR "\r\n");
    // ����������Ӆ���ؘ����ش�����̅��͆��Ѝ�δ���í�������ͭ�í��͆��Ѝ��
    if (sizeof(me["keymap"]) == 0) pl->delete("me_keymap");
    else pl->set("me_keymap", me["keymap"]);
    map_delete(EditedFiles, pl->GetProperty("me/sFileName"));
    pl->delete_temp("me"); // ̅������˕�ȴ���՜ט��ײ
} // _quit()


private void _refresh(object pl)        // ���é�Ȇ
{
    mapping me=pl->GetProperty("me");
    _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
} // _refresh()


private void _replace(object pl)        // �ٳ�ކ��
{
    _message(pl, "��җ/ކ���ٳ�(٘�ԗ�ENTER��Γ,���ؗڹ���):",
            "_replace_done", ECHO);
} // _replace()

protected void _replace_done(string str, object pl) // ���ކ���ٳ�
{
    mapping me=pl->GetProperty("me");

    _refresh_status(pl);
    if (str != "")
    {        // ��ׄ������ט
        if (sscanf(str, "%s %s", me["sSearch"],        me["sReplace"]) != 2)
        {        // ٩��ө���ٳ�����ٹ�����җ�ٳ���ӂ���ȴ�ٹ���ٳ�
            me["sSearch"] = str;
            me["sReplace"] = "";
        }
        _next_match(pl);
    }
    else        get_char("_input", HIDE, pl); // ��ϰ�®��ù�ׄ���ٳ�
} // _replace_done()


private void _right(object pl, int col, int extra)        // ��ͷ����ө��
{
    int    acc;
    mapping me=pl->GetProperty("me");


    if (sizeof(me["sText"])==0 || me["iS_Col"]+me["iCol"]-1>
            sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-1+extra)
        return;
    me["iCol"] += col;
    if (me["iS_Col"]+me["iCol"]-1 > sizeof(me["sText"][me["iS_Row"]+
                me["iRow"]-2])+extra)
        me["iCol"] = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
            me["iS_Col"]+1+extra;
    if (me["iCol"] > 80)
    {        // �𘊴�����ș����Э��Գ�����ψӬͷ���˵��Գ�
        // ������ 8 ��޽����������٩���˴����
        if (me["iE_Col"]+8 > sizeof(me["sText"][me["iS_Row"]+
                    me["iRow"]-2])+1)
            acc = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
                me["iE_Col"]+1;
        else        acc = 8;
        me["iCol"] = 80-acc+1;         // ��݃�𘊴������
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]+acc);
        return;
    }
    _refresh_status(pl);
} // _right()


private void _search(object pl)                // �ٳ���җ
{
    _message(pl, "̋ׄ����җ�ٳ���٘�ԗ� ENTER ��Γ)��", "_search_done",
        ECHO);
} // _search()


protected void _search_done(string str, object pl) // �����җ�ٳ�
{
    mapping me=pl->GetProperty("me");

    _refresh_status(pl);
    if (str != "")
    {        // ��ׄ������ט
        me["sSearch"] = str;        me["sReplace"] = 0;
        _next_match(pl);
    }
    else        get_char("_input", HIDE, pl); // ��ϰ�®��ù�ׄ���ٳ�
} // _search_done()


private void _tab(object pl)                 // Ȓ��׻�� TAB �������
{
    mapping me=pl->GetProperty("me");
    _right(pl, 8-(me["iCol"]-1)%8, 0);
} // _tab()


private void _undef_key(object pl)        // ��Γ����������Ӆ
{
    _message(pl, "̋ׄ��ԓ��Γ��Ӆ٫��\������٘�ԗ� ENTER ��Γ)��",
        "_undef_key_done", ECHO);
} // undef_key()


protected void _undef_key_done(string str, object pl) // ���ٹ��������
{
    mapping me=pl->GetProperty("me");
    if (str != "")        map_delete(me["keymap"], str);
    _message_done("", pl);
} // undef_key_done()


private void _up(object pl)                // ��ͷ����ө��
{
    int  acc;
    mapping me=pl->GetProperty("me");


    if (me["iRow"]==1 && me["iS_Row"]==1)        return;
    if (--me["iRow"] < 1)
    {        // ������ș����Э��Գ������������Գ�
        me["iS_Row"]--;
        me["iE_Row"]=me["iS_Row"]+22;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"], me["iS_Col"]);
        me["iRow"] = 1;
    }
    // ���������������������˴����
    if (me["iS_Col"]+me["iCol"]-1 > sizeof(me["sText"][me["iS_Row"]+
                me["iRow"]-2]))
        me["iCol"] = sizeof(me["sText"][me["iS_Row"]+me["iRow"]-2])-
            me["iS_Col"]+(me["is_Command_Mode"]?1:2);
    // ��������������������ө��
    if (me["iCol"]==0 && me["iS_Col"]==1) me["iCol"] = 1;
    if (me["iCol"] > 0) _refresh_status(pl);
    else
    {        // ������ș����Э��Գ������۩���Ȇ
        acc = me["iCol"];        me["iCol"] = 1;
        _refresh_screen(pl, me["iS_Row"], me["iE_Row"],
                me["iS_Col"]+acc-1);
    }
} // _up()


private void _write(object pl)                // ��ȳ󳆴���ȓ��
{
    _message(pl, "̋ׄ���ô�ȓ��٘�ԗ� ENTER ��נ�Ⱦд�ȓ)��",
        "_write_done", ECHO, 0);
} // _write()


protected void _write_done(string str, object pl, int quit) // �󳆴���
{
    string callback;
    mapping me=pl->GetProperty("me");

    if (str == "") str = me["sFileName"];
    else        // ��ȴ�ȓ
        str = absolute_path(pl->query("cwd"), str);
    if (quit) callback = "_quit";
    else callback = "_message_done";
    if (write_file(str, implode(me["sText"], "\n")+"\n", 1))
    {
        if (me["is_Modify"])
            log_file(LOGFILE, sprintf("[%s] %s %s %s\n",
                        ctime(time()), getuid(pl),
                        me["is_NewFile"]? "+": "*", str));
        me["is_Modify"] = me["is_NewFile"] = 0;
        me["sFileName"] = str;
        _message(pl, "�����Р�\��", callback, HIDE);
    }
    else
        _message(pl, "����׺����", callback, HIDE);
} // write_done()
