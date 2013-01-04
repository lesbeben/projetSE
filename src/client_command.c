#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "client_command.h"

//////////////////// //////////////////////////////////////
// COMMANDE PID
//////////////////////////////////////////////////////////
void pid_help() {
    printf("Commande PID\n");
    printf("  Usage : \n");
    printf("  \"PID numpid\"\n");
}

request_t* pid_request_func(const char* cmd) {
	char* tailptr[] = { NULL };
	pid_t pid = strtol(cmd + 3, tailptr, 0);
	if (errno != 0) {
		perror("strtol");
		return NULL;
	}
	if ((pid == 0) && (tailptr[0] != NULL)) {
		fprintf(stderr, "%s n'est pas un entier\n", cmd);
        pid_help();
		return NULL;
	}
	request_t* req = malloc(sizeof(request_t) + sizeof(pid_t));
	req->size = sizeof(request_t) + sizeof(pid_t);
	strncpy(req->cmdname, cmd, 3);
	req->clientpid = getpid();
	*((int*)req->data) = pid;
	return req;
}

request_cmd_t pid_request_cmd = {
    "PID", pid_request_func, pid_help
};

//////////////////// //////////////////////////////////////
// COMMANDE GID
//////////////////////////////////////////////////////////
void gid_help() {
    printf("Commande GID\n");
    printf("  Usage : \n");
    printf("  \"GID numgid\"\n");
}

request_t* gid_request_func(const char* cmd) {
	char* tailptr[] = { NULL };
	gid_t gid = strtol(cmd + 3, tailptr, 0);
	if (errno != 0) {
		perror("strtol");
		return NULL;
	}
	if ((gid == 0) && (tailptr[0] != NULL)) {
		fprintf(stderr, "%s n'est pas un entier\n", cmd);
        gid_help();
		return NULL;
	}
	request_t* req = malloc(sizeof(request_t) + sizeof(gid_t));
	req->size = sizeof(request_t) + sizeof(gid_t);
	strncpy(req->cmdname, cmd, 3);
	req->clientpid = getpid();
	*((int*)req->data) = gid;
	return req;
}

request_cmd_t gid_request_cmd = {
    "GID", gid_request_func, gid_help
};
