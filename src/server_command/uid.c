#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE GID
//////////////////////////////////////////////////////////
int uid_answer_func(request_t* req, char* buffer, size_t buffsize) {
	int res = 0;
	struct passwd* pw = getpwuid(*((int*) (req->data)));
	if (pw == NULL) {
		snprintf(buffer, buffsize
				, "Erreur lors de la lecture de l'UID : %d"
				, *((int*) (req->data)));
		fprintf(stderr, "%s\n", buffer);
		res = -1;
	} else {
		snprintf(buffer, buffsize, "%s", pw->pw_name);
	}
	return res;
}

answer_cmd_t uid_answer_cmd = {
    "UID", uid_answer_func
};

answer_cmd_t getCommand() {
	return uid_answer_cmd;
}

