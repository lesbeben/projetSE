#ifndef TPROJECT_h
#define PROJECT_h


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#define STRSIZ 512


const char* request_name = "request_stream";
/*
 * Préfixe du nom du tube nommé de réponse
 * Le nom du tube nommé sera de la forme :
 *   "answer + pid du client"
 */
const char* answer_prefix = "answer_stream";

typedef enum {
    UID,
    GID,
    PID
} request_type;

typedef struct {
	union {
		uid_t uid;
		gid_t gid;
        pid_t pid;
	} data;
	request_type type;
	int clientpid;
} request_t;

#endif
