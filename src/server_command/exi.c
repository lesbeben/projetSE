#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE EXI
//////////////////////////////////////////////////////////
int exi_answer_func(request_t* req, char* buffer, size_t buffsize) {
	strcpy(buffer, "");
	return -3;
}

answer_cmd_t exi_answer_cmd = {
    "EXI", exi_answer_func
};

answer_cmd_t getCommand() {
	return exi_answer_cmd;
}

