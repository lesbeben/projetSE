#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE CON
//////////////////////////////////////////////////////////
int con_answer_func(request_t* req, char* buffer, size_t buffsize) {
	strcpy(buffer, "");
	return 0;
}

answer_cmd_t con_answer_cmd = {
    "CON", con_answer_func
};

answer_cmd_t getCommand() {
	return con_answer_cmd;
}

