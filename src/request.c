#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include "client_command.h"
#include "server_command.h"
#include "request.h"

request_t* create_request(const char* cmd) {
	request_cmd_t req_cmd[] = {pid_request_cmd, gid_request_cmd};
	for (int i = 0; i < sizeof(req_cmd); i++) {
		if (strncmp(cmd, req_cmd[i].cmd_name, 3) == 0) {
			return req_cmd[i].request_cmd_func(cmd);
		}
	}
	fprintf(stderr, "Commande %s inconnue\n", cmd);
	return NULL;
}

void delete_request(request_t* req) {
	free(req);
}

int getanswer(request_t* req, char* buffer, size_t buffsize) {
	answer_cmd_t ans_cmd[] = {pid_answer_cmd, gid_answer_cmd};
	for (int i = 0; i < sizeof(ans_cmd); i++) {
		if (strncmp(req->cmdname, ans_cmd[i].cmd_name, 3) == 0) {
			return ans_cmd[i].answer_cmd_func(req, buffer, buffsize);
		}
	}
	fprintf(stderr, "Commande %s inconnue\n", req->cmdname);
	snprintf(buffer, buffsize
			, "Requete %s inconnue"
			, req->cmdname);
	return -1;
}
