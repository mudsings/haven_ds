#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

static void eventUpdate() {
        string newfile = "#include <lib.h>\n";
        newfile += "\n";
        newfile += "inherit LIB_DAEMON;\n";
        newfile += "\n";
        newfile += "static void eventUpdate() {}\n";
        newfile += "\n";
        newfile += "static void create() {\n";
        newfile += "    daemon::create();\n";
        newfile += "    call_out((: eventUpdate :), 60);\n";
        newfile += "}\n";

	rm("/cmds/creators/people.c");
	rm("/secure/daemon/i3router/socket_stuff.h");

        reload(CMD_D);

        PLAYERS_D->CompileCreList();
        PLAYERS_D->CompilePlayerList();

        rename("/secure/daemon/update.c", "/secure/daemon/update.patch");
	write_file("/secure/daemon/update.c",newfile,1);
    }

static void create() {
    daemon::create();
    call_out((: eventUpdate :), 2);
}


