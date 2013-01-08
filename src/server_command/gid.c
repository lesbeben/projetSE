#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <grp.h>
#include <string.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE GID
//////////////////////////////////////////////////////////
int gid_answer_func(request_t* req, char* buffer, size_t buffsize) {
	int res = 0;
	struct group* grp = getgrgid(*((int*) (req->data)));
	if (grp == NULL) {
		snprintf(buffer, buffsize
				, "Erreur lors de la lecture du gid : %d"
				, *((int*) (req->data)));
		fprintf(stderr, "%s\n", buffer);
		res = -1;
	} else {
		snprintf(buffer, buffsize, "%s", grp->gr_name);
	}
	return res;
}

answer_cmd_t gid_answer_cmd = {
    "GID", gid_answer_func
};

answer_cmd_t getCommand() {
	return gid_answer_cmd;
}

