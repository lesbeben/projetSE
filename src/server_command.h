#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include "request.h"

typedef struct {
	char cmd_name[4];
	int (*answer_cmd_func) (request_t*, char*, size_t);
} answer_cmd_t;

extern answer_cmd_t pid_answer_cmd;
extern answer_cmd_t gid_answer_cmd;

#endif
