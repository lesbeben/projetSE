#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE GID
//////////////////////////////////////////////////////////
void gid_help() {
    printf("Commande GID\n");
    printf("  Affiche le nom du groupe 'numgid'\n");
    printf("  Usage : \n");
    printf("    \"GID numgid\"\n");
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
	req->cmdname[3] = '\0';
	req->clientpid = getpid();
	*((int*)req->data) = gid;
	return req;
}

request_cmd_t gid_request_cmd = {
    "GID", gid_request_func, gid_help
};

request_cmd_t getCommand() {
	return gid_request_cmd;
}
