// This file written mostly by Tim Johnson (Tim@TimMUD)

static void send_error(string mud, string user, string errcode, string errmsg, mixed *info){
    if(!connected_muds[mud]){
	//trr("Can't send error to "+mud+" because they're not connected.");
	return;
    }
    log_file("router/server_log",timestamp()+" Sending error to "+mud+": "+errmsg+"\n");
    write_data(connected_muds[mud],({
	"error",
	5,
	router_name,
	0,
	mud, // mud name
	user, // user name
	errcode,
	errmsg,
	info
      }));
    return;
}
