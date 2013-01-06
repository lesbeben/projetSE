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
#include "stream_manager.h"
#include "project.h" //verifier les includes

//procedure de gestion des signaux
void signal_handler(int signum){ // a modifier
	printf("recu signal: %d", signum);
	manager_clean();
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
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
	
	//pour l'instant flux de question en dur
	stream_t req_str;
	stream_set_t sset;
	req_str = manager_getstream('fif');
	if (stream_open(req_str[i], request_name, O_WRONLY) < 0) { 
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	
	//on calcule le nom de flux de reponse
    sprintf(ans_name, "%s%i", answer_prefix, req->clientpid);
    //on cree et ouvre le flux de reponse (en dur pour l'instant
    stream_t ans_str = manager_getstream(req->answer_stream);
    if (stream_create(&ans_str, ans_name, BUFSIZ) < 0) { //quelle taille pour le create?
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
    if (stream_open(&ans_str, ans_name, O_RDONLY) < 0) { 
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	
	int res= 0
	while (res != -233) { //code exit
		//on lit la commande sur l'entree standard
		scanf("%s\n", &cmd_buf);
		//on cree la requete
		req = create_request(cmdbuf);
		//on envoie la question
		if (stream_write(&req_str, req, BUFSIZE) < 0) {
			raise(SIGTERM);
			//plantage
		}	
		
		//on recoit la reponse
		if (stream_read(&ans_str, &ans_buf, BUFSIZE) < 0) {
			raise(SIGTERM);
			//plantage
		}
		//on affiche la reponse
		printf("%s\n", ans_buf);
		//on efface la requete
		delete_request(req);
	}
	//close
	//unlink
	return EXIT_SUCCESS;
}
