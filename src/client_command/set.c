#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE SET
//////////////////////////////////////////////////////////
void set_help() {
}

request_t* set_request_func(const char* cmd) {
	request_t* req = malloc(sizeof(request_t) + 3 * sizeof(char));
	req->size = sizeof(request_t) + 3 * sizeof(char);
	strncpy(req->cmdname, cmd, 3);
	req->cmdname[3] = '\0';
	req->clientpid = getpid();
	char buf[512];
	strncpy(buf, cmd, 512);
	strtok(buf, " ");
	char* res = strtok(NULL, " ");
	if (res != NULL) {
		strncpy(((char*)req->data), res, 3);
	}
	return req;
}

request_cmd_t set_request_cmd = {
    "SET", set_request_func, set_help
};

request_cmd_t getCommand() {
	return set_request_cmd;
}
