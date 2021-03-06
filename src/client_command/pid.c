#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE PID
//////////////////////////////////////////////////////////
void pid_help() {
    printf("Commande PID\n");
    printf("  Affiche le nom du processus 'numpid'\n");
    printf("  Usage : \n");
    printf("    \"PID numpid\"\n");
}

request_t* pid_request_func(const char* cmd) {
	char* tailptr[] = { NULL };
	pid_t pid = strtol(cmd + 3, tailptr, 0);
	if ((errno == ERANGE && (pid == LONG_MAX || pid == LONG_MIN))
		   || (errno != 0 && pid == 0)) {
	    fprintf(stderr, "%s n'est pas un entier\n", cmd + 3);
	    pid_help();
	    return NULL;
	}

	request_t* req = malloc(sizeof(request_t) + sizeof(pid_t));
	req->size = sizeof(request_t) + sizeof(pid_t);
	strncpy(req->cmdname, cmd, 3);
	req->cmdname[3] = '\0';
	req->clientpid = getpid();
	*((int*)req->data) = pid;
	return req;
}

request_cmd_t pid_request_cmd = {
    "PID", pid_request_func, pid_help
};

request_cmd_t getCommand() {
	return pid_request_cmd;
}
