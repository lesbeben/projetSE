#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE UID
//////////////////////////////////////////////////////////
void uid_help() {
    printf("Commande UID\n");
    printf("  Affiche le nom de l'utilisateur 'numuid'\n");
    printf("  Usage : \n");
    printf("    \"UID numuid\"\n");
}

request_t* uid_request_func(const char* cmd) {
	char* tailptr[] = { NULL };
	uid_t uid = strtol(cmd + 3, tailptr, 0);
	if ((errno == ERANGE && (uid == LONG_MAX || uid == LONG_MIN))
		   || (errno != 0 && uid == 0)) {
	    fprintf(stderr, "%s n'est pas un entier\n", cmd + 3);
	    uid_help();
	    return NULL;
	}
	
	request_t* req = malloc(sizeof(request_t) + sizeof(uid_t));
	req->size = sizeof(request_t) + sizeof(uid_t);
	strncpy(req->cmdname, cmd, 3);
	req->cmdname[3] = '\0';
	req->clientpid = getpid();
	*((int*)req->data) = uid;
	return req;
}

request_cmd_t uid_request_cmd = {
    "UID", uid_request_func, uid_help
};

request_cmd_t getCommand() {
	return uid_request_cmd;
}
