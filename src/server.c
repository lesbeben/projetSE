#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "stream_manager.h"
#include "stream_set.h"
#include "server_request.h"
#include "signal.h"
#include "child_list.h"
#include "project.h"

void help(){
	printf("./server \n"); // a changer
}


//procedure de reponse
void process_request(request_t *req) {
    char ans_name[STRSIZ];
    char buf[BUFSIZ];
    //on calcule le nom de flux de reponse
    sprintf(ans_name, "%s%i", answer_prefix, req->clientpid);
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

int main(int argc, char **argv) {
	//on initialise les signaux
	setSignals();
	// On initialise le stream manager
	manager_init();
	// On initialise le request manager
	request_manager_init();
	
	//on cree en dur les types de stream
	stream_t req_str[] = {
		manager_getstream("FIF")
		, manager_getstream("MQU")
		, manager_getstream("SHM")
	};
	stream_set_t sset;
	// On initialise l'ensemble de flux.
	stream_set_clear(&sset);
	int n = 0;
	char buffer[BUFSIZ];
	//creer map (clientpid, tube)

	//on cree et ouvre les trois streams et on les ajoute au set
	for (int i = 0; i < 3; ++i) {
		if (stream_create(&req_str[i], request_name, BUFSIZ) < 0) { //quelle taille pour le create?
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
		if (stream_open(&req_str[i], request_name, O_RDONLY) < 0) { 
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
		stream_set_add(&sset, &req_str[i]);
	}

	//on lance le serveur
	while ((stream_set_select(&sset) > 0) && !isDone()) {
		if (errno == 0) {
			//pour tous les types de flux qui sont actifs
			for (int i = 0; i < 3; ++i) {
				if ((n =stream_set_isset(&sset, &req_str[i]) > 0)) {
					//on lit dans le flux
					if (stream_read(&req_str[i], buffer, BUFSIZ) < 0) { // buff de buffsize
						raise(SIGTERM);
						//plantage
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
								//on gere les requetes
								while(((n = read(tube[0], buffer, BUFSIZ)) > 0) && (errno != EINTR)){ // Pas req mais buffer de bufsize
									process_request((request_t*) buffer);
								}
								//on verifie aue tout s'est bien passe
								if (n == -1) {
									perror("read");
									raise(SIGTERM);
								}
								//on ferme la sortie du tube
								close(tube[0]);
								//on ferme le sous-serveur
								raise(SIGTERM);
								
							default:
								//on ferme la sortie du tube
								close(tube[0]);
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
							//plantage
						}						
					}
				}
				//on teste les erreur de stream_set_isset
				if (n <  0){
					//on gere l'erreur
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
			fprintf(stderr, "Impossible de fermer un flux\n");
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

