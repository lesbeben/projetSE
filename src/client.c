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
#include <fcntl.h>

#include "stream_manager.h"
#include "stream_set.h"
#include "client_request.h"
#include "signal.h"
#include "project.h"


void send(stream_t* stream, char* cmd, int size);


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
	char ans_stream_type[STRSIZ];
	sprintf(ans_stream_type, "%s", "FIF");
	
	// On ouvre le flux de requetes
	stream_t req_str;
	req_str = manager_getstream(ans_stream_type);
	if (stream_open(&req_str, getRequestName(), O_WRONLY) < 0) {
		raise(SIGTERM);
	}
	
	//on calcule le nom de flux de reponse
    sprintf(ans_name, "%s%i", getAnswerPrefix(), getpid());
    //on cree et ouvre le flux de reponse (en dur pour l'instant
    stream_t ans_str = manager_getstream("FIF");
    if (stream_create(&ans_str, ans_name, BUFSIZ-1) < 0) {
		raise(SIGTERM);
	} else {
		if (stream_open(&ans_str, ans_name, O_RDONLY) < 0) {
			raise(SIGTERM);
		}
	}
	// On tente de se connecter
	if (!isDone()) {
		send(&req_str, "CON", strlen("CON"));
		struct timeval timeout = {10, 0};
		stream_set_t sset;
		stream_set_clear(&sset);
		stream_set_add(&sset, &ans_str);
		if (stream_set_select(&sset, &timeout) > 0) {
			if (stream_read(&ans_str, &ans_buf, BUFSIZ-1) < 0) {
				fprintf(stderr, "Erreur de reception des donnees\n");
			}
		}
		if ((timeout.tv_sec == 0) && (timeout.tv_usec == 0)) {
			fprintf(stderr, "Impossible de se connecter au serveur\n");
			done();
		}
	}
	
	while (!isDone()) {
	    printf("Entrez une commande : ");
	    fflush(stdout);
		//on lit la commande sur l'entree standard
		fgets(cmd_buf, BUFSIZ, stdin);
		if (errno == 0) {
			if (strncmp(cmd_buf, "EXIT", 4) == 0) {
				send(&req_str, cmd_buf, strlen(cmd_buf));
				done();
				continue;
			} else if (strncmp(cmd_buf, "SETIN", 5) == 0) {
				char buf[STRSIZ];
				strncpy(buf, cmd_buf, STRSIZ);
				strtok(buf, " ");
				char* res = strtok(NULL, " ");
				if (res != NULL) {
					strncpy(ans_stream_type, res, 3);
					ans_stream_type[3] = '\0';
					stream_close(&ans_str);
					stream_unlink(&ans_str, ans_name);
					ans_str = manager_getstream(ans_stream_type);
					stream_create(&ans_str, ans_name, BUFSIZ-1);
					stream_open(&ans_str, ans_name, O_RDONLY);
					send(&req_str, cmd_buf, strlen(cmd_buf));
					printf("Changement du mode de reponse en %s\n", ans_stream_type);
					continue;
				}
			} else if (strncmp(cmd_buf, "SETOUT", 6) == 0) {
				char buf[STRSIZ];
				strncpy(buf, cmd_buf, STRSIZ);
				strtok(buf, " ");
				char* res = strtok(NULL, " ");
				if (res != NULL) {
					char name[STRSIZ];
					strncpy(name, res, 3);
					name[3] = '\0';
					stream_close(&req_str);
					req_str = manager_getstream(name);
					stream_open(&req_str, getRequestName(), O_WRONLY);
					printf("Changement du mode de requete en %s\n", name);
					continue;
				}
			} else if (strncmp(cmd_buf, "HELP", 4) == 0) {
				request_manager_helpAll();
				continue;
			}
			//on cree la requete
			req = create_request(cmd_buf);
			if (req != NULL) {
				//on envoie la question
				if (stream_write(&req_str, req, BUFSIZ) < 0) {
					raise(SIGTERM);
				}
				//on recoit la reponse
				int n = 0;
				// On attend un maximum de 10s
				struct timeval timeout = {10, 0};
				stream_set_t sset;
				stream_set_clear(&sset);
				stream_set_add(&sset, &ans_str);
				if (stream_set_select(&sset, &timeout) > 0) {
					if ((n = stream_read(&ans_str, &ans_buf, BUFSIZ-1)) < 0) {
						fprintf(stderr, "Erreur de reception des donnees\n");
					}
					//on affiche la reponse
					ans_buf[n] = '\0';
					printf("%s\n", ans_buf);					
				}
				if ((timeout.tv_sec == 0) && (timeout.tv_usec == 0)) {
					fprintf(stderr, "Timeout de reception de donnees\n");
				}
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

/**
 * Construit une requete et l'envoie
 */
void send(stream_t* stream, char* cmd, int size) {
	request_t* req = create_request(cmd);
	if (req != NULL) {
		if (stream_write(stream, req, BUFSIZ) < 0) {
			fprintf(stderr, "Erreur d'envoie d'une requete\n");
		}
		delete_request(req);
	}
}
