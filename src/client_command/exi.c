#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE EXI
//////////////////////////////////////////////////////////
void exi_help() {
}

request_t* exi_request_func(const char* cmd) {
	request_t* req = malloc(sizeof(request_t));
	req->size = sizeof(request_t);
	strncpy(req->cmdname, cmd, 3);
	req->cmdname[3] = '\0';
	req->clientpid = getpid();
	return req;
}

request_cmd_t exi_request_cmd = {
    "EXI", exi_request_func, exi_help
};

request_cmd_t getCommand() {
	return exi_request_cmd;
}
