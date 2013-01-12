#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE TIME
//////////////////////////////////////////////////////////
void time_help() {
    printf("Commande TIM\n");
    printf("  Affiche l'heure du serveur\n");
    printf("  Usage : \n");
    printf("    \"TIM\"\n");
}

request_t* time_request_func(const char* cmd) {
	request_t* req = malloc(sizeof(request_t));
	req->size = sizeof(request_t);
	strncpy(req->cmdname, cmd, 3);
	req->cmdname[3] = '\0';
	req->clientpid = getpid();
	return req;
}

request_cmd_t time_request_cmd = {
    "TIM", time_request_func, time_help
};

request_cmd_t getCommand() {
	return time_request_cmd;
}
