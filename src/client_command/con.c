#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE CON
//////////////////////////////////////////////////////////
void con_help() {
}

request_t* con_request_func(const char* cmd) {
	request_t* req = malloc(sizeof(request_t));
	req->size = sizeof(request_t);
	strncpy(req->cmdname, cmd, 3);
	req->cmdname[3] = '\0';
	req->clientpid = getpid();
	return req;
}

request_cmd_t con_request_cmd = {
    "CON", con_request_func, con_help
};

request_cmd_t getCommand() {
	return con_request_cmd;
}
