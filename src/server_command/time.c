#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <grp.h>
#include <string.h>
#include <time.h>
#include "command.h"

//////////////////// //////////////////////////////////////
// COMMANDE TIME
//////////////////////////////////////////////////////////
int time_answer_func(request_t* req, char* buffer, size_t buffsize) {
	time_t heure;
	struct tm* h;
	
	heure = time(NULL);
	if (heure == -1) {
		perror("time");
		snprintf(buffer, buffsize
				, "Erreur lors de la lecture de l'heure");
		fprintf(stderr, "%s\n", buffer);
		return -1;
	}
	
	h = localtime(&heure);
	
	if (h != NULL) {
		snprintf(buffer, buffsize
				, "%s", asctime(h));
	} else {
		perror("localtime");
		snprintf(buffer, buffsize
				, "Erreur lors de la lecture de l'heure");
		fprintf(stderr, "%s\n", buffer);
		return -1;
	}

	return 0;
}

answer_cmd_t time_answer_cmd = {
    "TIM", time_answer_func
};

answer_cmd_t getCommand() {
	return time_answer_cmd;
}

