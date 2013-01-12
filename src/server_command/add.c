#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE ADD
//////////////////////////////////////////////////////////
int add_answer_func(request_t* req, char* buffer, size_t buffsize) {
	int res = 0;
	int x = *((int*) (req->data));
	int y = *((int*) (req->data) + 1);
	
	snprintf(buffer, buffsize, "%d", x + y);
	
	return res;
}

answer_cmd_t add_answer_cmd = {
    "ADD", add_answer_func
};

answer_cmd_t getCommand() {
	return add_answer_cmd;
}

