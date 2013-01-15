#include <stdlib.h>
#include <errno.h>
#include "server_request.h"
#include "signal.h"
#include "project.h"
#include "child_process.h"
#include "stream_manager.h"

//procedure de reponse
void process_request(request_t *req) {
    char ans_name[STRSIZ];
    char buf[BUFSIZ];
    //on calcule le nom de flux de reponse
    sprintf(ans_name, "%s%i", getAnswerPrefix(), req->clientpid);
    //on ouvre le flux de reponse
    stream_t ans_str = manager_getstream(req->answer_stream);
    if (stream_open(&ans_str, ans_name, O_WRONLY) < 0) { 
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	//on met la reponse dans le buffer
	get_answer(req, buf, BUFSIZ); // ou un switch sur les cmdname?
	//on envoie la reponse
	if (stream_write(&ans_str, buf, BUFSIZ-1) < 0) {
		raise(SIGTERM);
		//plantage
	}							
    //on ferme le  flux de reponse
    if (stream_close(&ans_str) < 0) {
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
}

void createChild(int pipe) {
	char buffer[BUFSIZ];
	int n = 0;	
	//on gere les requetes
	while (!isDone()) {
		while((n = read(pipe, buffer, BUFSIZ)) > 0) {
			process_request((request_t*) buffer);
		}
		//on verifie que tout s'est bien passe
		if ((n == -1) && (errno !=  EINTR) && (errno != 0)) {
			perror("read");
		}
	}
}
