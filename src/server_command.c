#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <grp.h>
#include <string.h>
#include "server_command.h"

//////////////////// //////////////////////////////////////
// COMMANDE PID
//////////////////////////////////////////////////////////
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

int pid_answer_func(request_t* req, char* buffer, size_t buffsize) {
	int res = getpidname(*((int*) (req->data)), buffer, buffsize);
	if (res == -1) {
		snprintf(buffer, buffsize
				, "Erreur lors de la lecture du pid : %d"
				, *((int*) (req->data)));
		fprintf(stderr, "%s\n", buffer);
	}
	return res;
}

answer_cmd_t pid_answer_cmd = {
    "PID", pid_answer_func
};

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

//////////////////// //////////////////////////////////////
// FONCTION PUBLIQUE
//////////////////////////////////////////////////////////

