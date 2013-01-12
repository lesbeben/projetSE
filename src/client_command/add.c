#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE ADD
//////////////////////////////////////////////////////////
void add_help() {
    printf("Commande ADD\n");
    printf("  Additionne 2 entiers x et y\n");
    printf("  Usage : \n");
    printf("    \"ADD x y\"\n");
}

request_t* add_request_func(const char* cmd) {
	char* tailptr;
	char* tailptr2;
	int x = 0;
	int y = 0;
	x = strtol(cmd + 3, &tailptr, 0);
	if ((errno == ERANGE && (x == LONG_MAX || x == LONG_MIN))
		   || (errno != 0 && x == 0)) {
	    fprintf(stderr, "%s n'est pas un entier\n", cmd + 3);
	    add_help();
	    return NULL;
	}
	errno = 0;
	y = strtol(tailptr, &tailptr2, 0);
	if ((errno == ERANGE && (y == LONG_MAX || y == LONG_MIN))
		   || (errno != 0 && y == 0)) {
	    fprintf(stderr, "%s n'est pas un entier\n", tailptr2);
	    add_help();
	    return NULL;
	}
	errno = 0;
	request_t* req = malloc(sizeof(request_t) + 2 * sizeof(int));
	req->size = sizeof(request_t) + 2 * sizeof(int);
	strncpy(req->cmdname, cmd, 3);
	req->cmdname[3] = '\0';
	req->clientpid = getpid();
	*((int*)req->data) = x;
	*((int*)req->data + 1) = y;
	return req;
}

request_cmd_t add_request_cmd = {
    "ADD", add_request_func, add_help
};

request_cmd_t getCommand() {
	return add_request_cmd;
}
