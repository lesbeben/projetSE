#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
#include "request.h"

int getpidname(pid_t pid, char* buffer, int bufferSize) {
    int res = 0;
    char name[20];
    printf("%d\n", pid);
    sprintf(name, "/proc/%d/comm", pid);
    FILE* f = fopen(name, "r");
    if (f == NULL) {
        res = -1;
    } else {
        if (fgets(buffer, bufferSize, f) == NULL) {
            res = -1;
        }
        int n = strlen(buffer);
        buffer[n - 1] = '\0';
        res = fclose(f);
    }
    return res;
}

typedef request_t* (request_cmd_func) (const char*);
typedef int (answer_cmd_func) (request_t*, char*, size_t);

typedef struct {
	char cmd_name[4];
	request_cmd_func* req_cmd;
} request_cmd_t;

typedef struct {
	char cmd_name[4];
	answer_cmd_func* ans_cmd;
} answer_cmd_t;

//////////////////////////////////////////////////////////
// Fonctions de requetes
//////////////////////////////////////////////////////////
request_t* pid_request_cmd(const char* cmd) {
	char* tailptr[] = { NULL };
	pid_t pid = strtol(cmd + 3, tailptr, 0);
	if (errno != 0) {
		perror("strtol");
		return NULL;
	}
	if ((pid == 0) && (tailptr[0] != NULL)) {
		fprintf(stderr, "%s n'est pas un entier\n", cmd);
		return NULL;
	}
	request_t* req = malloc(sizeof(request_t) + sizeof(pid_t));
	req->size = sizeof(request_t) + sizeof(pid_t);
	strncpy(req->cmdname, cmd, 3);
	req->clientpid = getpid();
	*((int*)req->data) = pid;
	return req;
}

request_t* gid_request_cmd(const char* cmd) {
	char* tailptr[] = { NULL };
	gid_t gid = strtol(cmd + 3, tailptr, 0);
	if (errno != 0) {
		perror("strtol");
		return NULL;
	}
	if ((gid == 0) && (tailptr[0] != NULL)) {
		fprintf(stderr, "%s n'est pas un entier\n", cmd);
		return NULL;
	}
	request_t* req = malloc(sizeof(request_t) + sizeof(gid_t));
	req->size = sizeof(request_t) + sizeof(gid_t);
	strncpy(req->cmdname, cmd, 3);
	req->clientpid = getpid();
	*((int*)req->data) = gid;
	return req;
}

//////////////////////////////////////////////////////////
// Fonctions de reponses
//////////////////////////////////////////////////////////
int pid_answer_cmd(request_t* req, char* buffer, size_t buffsize) {
	int res = getpidname(*((int*) (req->data)), buffer, buffsize);
	if (res == -1) {
		snprintf(buffer, buffsize
				, "Erreur lors de la lecture du pid : %d"
				, *((int*) (req->data)));
		fprintf(stderr, "%s\n", buffer);
	}
	return res;
}

int gid_answer_cmd(request_t* req, char* buffer, size_t buffsize) {
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

//////////////////////////////////////////////////////////
// Fonctions de publiques
//////////////////////////////////////////////////////////

request_t* create_request(const char* cmd) {
	request_cmd_t req_cmd[] = {
		{"PID", pid_request_cmd},
		{"GID", gid_request_cmd}
	};
	for (int i = 0; i < sizeof(req_cmd); i++) {
		if (strncmp(cmd, req_cmd[i].cmd_name, 3) == 0) {
			return req_cmd[i].req_cmd(cmd);
		}
	}
	fprintf(stderr, "Commande %s inconnue\n", cmd);
	return NULL;
}

void delete_request(request_t* req) {
	free(req);
}

int getanswer(request_t* req, char* buffer, size_t buffsize) {
	answer_cmd_t ans_cmd[] = {
		{"PID", pid_answer_cmd},
		{"GID", gid_answer_cmd}
	};
	for (int i = 0; i < sizeof(ans_cmd); i++) {
		if (strncmp(req->cmdname, ans_cmd[i].cmd_name, 3) == 0) {
			return ans_cmd[i].ans_cmd(req, buffer, buffsize);
		}
	}
	fprintf(stderr, "Commande %s inconnue\n", req->cmdname);
	snprintf(buffer, buffsize
			, "Requete %s inconnue"
			, req->cmdname);
	return -1;
}
