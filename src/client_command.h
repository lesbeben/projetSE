#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include "request.h"

typedef struct {
	char cmd_name[4];
	request_t* (*request_cmd_func) (const char*);
    void (*help) ();
} request_cmd_t;

#endif
