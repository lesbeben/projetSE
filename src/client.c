/*
 * client.c
 * 
 * Copyright 2013 Benjamin Zigh <benj@minzigh.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "stream_manager.h"
#include "stream_set.h"
#include "client_request.h"
#include "project.h" //verifier les includes

//procedure de gestion des signaux
void signal_handler(int signum){ // a modifier
	printf("recu signal: %d\n", signum);
	manager_clean();
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	// On initialise le stream_manager
	manager_init();
	// On initialise le request_manager
	request_manager_init();
	request_t *req = NULL;
	char ans_name[STRSIZ];
	char ans_buf[BUFSIZ];
	char cmd_buf[BUFSIZ];
	//on initialise sigaction
	struct sigaction action;
	action.sa_handler = signal_handler;
	action.sa_flags = 0;
	if (sigfillset(&action.sa_mask) == -1) {
		perror("sigfillset");
		raise(SIGTERM);
	}
    int signals[] = {SIGINT, SIGTERM};
    for (int i = 0; i < 2; i++) {
        if (sigaction(signals[i], &action, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
    }
	
	//pour l'instant flux de question en dur
	stream_t req_str;
	req_str = manager_getstream("shm");
	if (stream_open(&req_str, request_name, O_WRONLY) < 0) { 
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	
	//on calcule le nom de flux de reponse
    sprintf(ans_name, "%s%i", answer_prefix, getpid());
    //on cree et ouvre le flux de reponse (en dur pour l'instant
    stream_t ans_str = manager_getstream("fif");
    if (stream_create(&ans_str, ans_name, BUFSIZ-1) < 0) { //quelle taille pour le create?
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
    if (stream_open(&ans_str, ans_name, O_RDONLY) < 0) { 
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	
	int res= 0;
	while (res != -233) { //code exit
	    printf("Entrez une commande : ");
	    fflush(stdout);
		//on lit la commande sur l'entree standard
		fgets(cmd_buf, BUFSIZ, stdin);
		//on cree la requete
		req = create_request(cmd_buf);
		if (req != NULL) {
			strncpy(req->answer_stream, "fif", 3);
			//on envoie la question
			if (stream_write(&req_str, req, BUFSIZ-1) < 0) {
				raise(SIGTERM);
				//plantage
			}
			//on recoit la reponse
			int n = 0;
			if ((n = stream_read(&ans_str, &ans_buf, BUFSIZ-1)) < 0) {
				raise(SIGTERM);
				//plantage
			}
			//on affiche la reponse
			ans_buf[n] = '\0';
			printf("%s\n", ans_buf);
			//on efface la requete
			delete_request(req);
		}
	}
	if (stream_close(&req_str) < 0) {
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	if (stream_close(&ans_str) < 0) {
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	if (stream_unlink(&ans_str, ans_name) < 0) { 
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	// On close le stream_manager
	manager_close();
	// On close le request_manager
	request_manager_close();
	return EXIT_SUCCESS;
}
