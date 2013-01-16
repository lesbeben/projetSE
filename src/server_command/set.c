#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE SET
//////////////////////////////////////////////////////////
int set_answer_func(request_t* req, char* buffer, size_t buffsize) {
	if (buffsize < 4) {
		return -1;
	}
	strncpy(buffer, ((char*)req->data), 3);
	buffer[3] = '\0';
	return -2;
}

answer_cmd_t set_answer_cmd = {
    "SET", set_answer_func
};

answer_cmd_t getCommand() {
	return set_answer_cmd;
}

