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
#include "signal.h"
#include "project.h"

int main(int argc, char **argv) {
	// On initialise les signaux
	setSignals();
	// On initialise le stream_manager
	manager_init();
	// On initialise le request_manager
	request_manager_init();
	
	request_t *req = NULL;
	char ans_name[STRSIZ];
	char ans_buf[BUFSIZ];
	char cmd_buf[BUFSIZ];
	
	//pour l'instant flux de question en dur
	stream_t req_str;
	req_str = manager_getstream("SHM");
	if (stream_open(&req_str, getRequestName(), O_WRONLY) < 0) { 
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	
	//on calcule le nom de flux de reponse
    sprintf(ans_name, "%s%i", getAnswerPrefix(), getpid());
    //on cree et ouvre le flux de reponse (en dur pour l'instant
    stream_t ans_str = manager_getstream("FIF");
    if (stream_create(&ans_str, ans_name, BUFSIZ-1) < 0) { //quelle taille pour le create?
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
    if (stream_open(&ans_str, ans_name, O_RDONLY) < 0) { 
		//ajouter gestion des cas -1 -2
		raise(SIGTERM);
	}
	
	while (!isDone()) {
	    printf("Entrez une commande : ");
	    fflush(stdout);
		//on lit la commande sur l'entree standard
		fgets(cmd_buf, BUFSIZ, stdin);
		if (errno == 0) {
			//on cree la requete
			req = create_request(cmd_buf);
			if (req != NULL) {
				strncpy(req->answer_stream, "FIF", 3);
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
		errno = 0;
	}
	
	if (stream_close(&req_str) < 0) {
		fprintf(stderr, "Impossible de fermer le flux de requetes\n");
	}
	if (stream_close(&ans_str) < 0) {
		fprintf(stderr, "Impossible de fermer le flux de reponses\n");
	}

	// Le manager s'occupe de supprimer tous les flux créés.
	manager_clean();
	// On close le stream_manager
	manager_close();
	// On close le request_manager
	request_manager_close();
	
	return EXIT_SUCCESS;
}
