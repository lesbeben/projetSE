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
#include "signal.h"

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
static void signal_handler(int signum){
	switch(signum) {
		case SIGTERM:
		case SIGQUIT:
			done();
			break;
		case SIGCHLD:
		
		default:
			break;
	}
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
