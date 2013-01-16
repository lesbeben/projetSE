/**
 * Implémente les signaux pour la gestion dans le server.
 * Membres publiques : 
 *   - done()
 *   - isDone(int)
 *   - setSignals()
 * 
 * Membres privés : 
 *   - is_done
 *   - signal_handler(int)
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include "signal.h"
#include "child_list.h"

/*
 * Variable globale permettant de mettre fin au programme.
 */
static int is_done = 0;

void done() {
	is_done = 1;
}

int isDone() {
	return is_done;
}

/*
 * Procdédure de gestion des signaux.
 */
static void signal_handler(int signum) {
	int r;
	int olderr = errno;
	errno = 0;	
	switch(signum) {
		case SIGQUIT:
		case SIGINT:
			kill(0, SIGTERM);
		case SIGTERM:
			done();
			break;
		case SIGCHLD:
			do {
				r = waitpid(-1, NULL, WNOHANG);
				removeChild(r);
			} while (r > 0);
			break;
		default:
			break;
	}
	errno = 0;
}

void setSignals() {
	struct sigaction action;
	action.sa_handler = signal_handler;
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
