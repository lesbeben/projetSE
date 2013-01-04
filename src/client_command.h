#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include "request.h"

typedef struct {
	char cmd_name[4];
	request_t* (*request_cmd_func) (const char*);
    void (*_help) ();
} request_cmd_t;

extern request_cmd_t pid_request_cmd;
extern request_cmd_t gid_request_cmd;

#endif
