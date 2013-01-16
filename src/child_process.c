#include <stdlib.h>
#include <errno.h>
#include "server_request.h"
#include "signal.h"
#include "project.h"
#include "child_process.h"
#include "stream_manager.h"

/*
 * Procdédure de gestion des signaux.
 */
static void _signal_handler(int signum) {
	switch(signum) {
		case SIGQUIT:
		case SIGINT:
		case SIGTERM:
			done();
			break;
		default:
			break;
	}
}

//procedure de reponse
static void process_request(stream_t* stream, request_t *req) {
    char buf[BUFSIZ];
    
	//on met la reponse dans le buffer
	int n = get_answer(req, buf, BUFSIZ);
	if (n == -2) {
		char ans_name[STRSIZ];
		//on calcule le nom de flux de reponse
		sprintf(ans_name, "%s%i", getAnswerPrefix(), req->clientpid);
		
		if (stream_close(stream) < 0) {
			fprintf(stderr, "Erreur de fermeture d'un flux");
		}
		*stream = manager_getstream(buf);
		if (stream_open(stream, ans_name, O_WRONLY) < 0) {
			raise(SIGTERM);
		}
		printf("Changement de communication avec %d en %s\n", req->clientpid, buf);
	} else if (n == -3) {
		printf("Terminaison du client %d\n", req->clientpid);
		done();
	} else if (n >= 0) {
		//on envoie la reponse
		if (stream_write(stream, buf, BUFSIZ-1) < 0) {
			raise(SIGTERM);
			//plantage
		}		
	}
}

static void _setSignals() {
	struct sigaction action;
	action.sa_handler = _signal_handler;
	action.sa_flags = 0;
	if (sigfillset(&action.sa_mask) == -1) {
		perror("sigfillset");
		raise(SIGTERM);
	}
    int signals[] = {SIGQUIT, SIGINT, SIGTERM, SIGCHLD};
    for (int i = 0; i < 4; i++) {
        if (sigaction(signals[i], &action, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
    }
}

void createChild(int pipe, pid_t clientPid) {
	_setSignals();
	char buffer[BUFSIZ];
	int n = 0;
	char ans_name[STRSIZ];
	stream_t ans_str = manager_getstream("FIF");
    //on calcule le nom de flux de reponse
    sprintf(ans_name, "%s%i", getAnswerPrefix(), clientPid);
    if (stream_open(&ans_str, ans_name, O_WRONLY) < 0) {
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	//on gere les requetes
	while (!isDone()) {
		if ((n = read(pipe, buffer, BUFSIZ)) > 0) {
			process_request(&ans_str, (request_t*) buffer);
		}
		//on verifie que tout s'est bien passe
		if ((n == -1) && (errno !=  EINTR) && (errno != 0)) {
			perror("read");
			done();
		}
	}
	
    //on ferme le  flux de reponse
    if (stream_close(&ans_str) < 0) {
		fprintf(stderr, "Erreur de fermeture d'un flux");
	}
}
