#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "stream_manager.h"
#include "stream_set.h"
#include "server_request.h"
#include "signal.h"
#include "child_list.h"
#include "child_process.h"
#include "project.h"


int main(int argc, char **argv) {
	//on initialise les signaux
	setSignals();
	// On initialise le stream manager
	manager_init();
	// On initialise le request manager
	request_manager_init();
	
	//on cree en dur les types de stream
	char* stream_type_name[] = {"FIF", "MQU", "SHM"};
	stream_t req_str[] = {
		manager_getstream(stream_type_name[0])
		, manager_getstream(stream_type_name[1])
		, manager_getstream(stream_type_name[2])
	};
	stream_set_t sset;
	// On initialise l'ensemble de flux.
	stream_set_clear(&sset);
	int n = 0;
	char buffer[BUFSIZ];
	//creer map (clientpid, tube)

	//on cree et ouvre les trois streams et on les ajoute au set
	for (int i = 0; i < 3; ++i) {
		if (stream_create(&req_str[i], getRequestName(), BUFSIZ) < 0) {
			raise(SIGTERM);
			break;
		} else {
			if (stream_open(&req_str[i], getRequestName(), O_RDONLY) < 0) { 
				raise(SIGTERM);
				break;
			} else {
				stream_set_add(&sset, &req_str[i]);
			}
		}
	}

	//on lance le serveur
	while (!isDone()) {
		if (stream_set_select(&sset, NULL) > 0) {
			//pour tous les types de flux qui sont actifs
			for (int i = 0; i < 3; ++i) {
				if ((n = stream_set_isset(&sset, &req_str[i]) > 0)) {
					printf("Reception de donnees sur %s\n", stream_type_name[i]);
					//on lit dans le flux
					if (stream_read(&req_str[i], buffer, BUFSIZ) < 0) {
						raise(SIGTERM);
						continue;
					}
					request_t* req = (request_t*) buffer;
					pid_t clientPid = req->clientpid;
					//S'il n'y a pas de processus de traitement associé à ce client
					if (!childExist(clientPid)) {
						//on cree le tube serveur/sous-serveur
						int tube[2];
						if (pipe(tube) == -1) {
							perror("pipe");
							raise(SIGTERM);
						}
						switch(fork()) {
							case -1:
								break;
							case 0:
								//on ferme l'entree du tube
								close(tube[1]);
								// On créé un processus de traitement
								createChild(tube[0], clientPid);
								//on ferme la sortie du tube
								close(tube[0]);
								//on ferme le sous-serveur
								exit(EXIT_SUCCESS);
							default:
								//on ferme la sortie du tube
								close(tube[0]);
								printf("Connection d'un nouveau client : %d\n", clientPid);
								addChild(clientPid, tube[1]);
								break;
						}
					}
					//on ecrit dans le tube
					int tube = getPipe(clientPid);
					if (tube != -1) {
						if (write(tube, buffer, BUFSIZ) < 0) {
							perror("write");
							raise(SIGTERM);
						}
					}
				}
			}
		}
		errno = 0;
		// On ré-initialise l'ensemble de flux
		stream_set_clear(&sset);
		for (int i = 0; i < 3; ++i) {
			stream_set_add(&sset, &req_str[i]);
		}
	}
	//On close tous les flux
	for (int i = 0; i < 3; ++i) {
		if (stream_close(&req_str[i]) < 0) {
			fprintf(stderr, "Impossible de fermer un flux %d\n", i);
		}
	}
	
	childsClean();
	// Le manager s'occupe de supprimer tous les flux créés.
	manager_clean();
	// On close le stream_manager
	manager_close();
	// On close le request_manager
	request_manager_close();
	
	return EXIT_SUCCESS;
}

