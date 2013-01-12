#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "stream_manager.h"
#include "stream_set.h"
#include "server_request.h"
#include "project.h" //verifier les includes


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

//procedure de gestion des signaux
void signal_handler(int signum){ // a modifier
	printf("recu signal: %d\n", signum);
	manager_clean();
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {

	//if (argc != 2){ // ou pas
		//help();
		//exit(EXIT_FAILURE);
	//}
	
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
	manager_init();
	// On initialise le request_manager
	request_manager_init();
	//on cree en dur les types de stream
	stream_t req_str[] ={ manager_getstream("fif"), manager_getstream("mq"), manager_getstream("shm")};//faire des constantes de type TUBE SHM MQ
	stream_set_t sset;
	// Faire un clear du set
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
	while (stream_set_select(&sset) > 0) {
		//pour tous les types de flux qui sont actifs
		for (int i = 0; i < 3; ++i) {
			if ((n =stream_set_isset(&sset, &req_str[i]) > 0)) {
				//si client pid pas dans map
					
					//ajouter (pid,tube) a la map
					
					//on cree le tube serveur/sous-serveur
					int tube[2];
					if (pipe(tube) == -1) {
						perror("pipe");
						exit(EXIT_FAILURE);
					}
					 
					switch(fork()) {
						case -1:
							//envoyer message de plantage a tous les tubes de la map
							
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
							
							//on lit dans le flux
							if (stream_read(&req_str[i], buffer, BUFSIZ) < 0) { // buff de buffsize
								raise(SIGTERM);
								//plantage
							}
							
							//on ecrit dans le tube
							if (write(tube[1], buffer, BUFSIZ) < 0) {
								perror("write");
								raise(SIGTERM);
								//plantage
							}
							
							//on refait un tour
							break;
					}
				//si clientpid dans la map
					//faire suivre la request dans le tube du sous-serveur
			}
			//on teste les erreur de stream_set_isset
			if (n <  0){
				//on gere l'erreur
			}
		}
		// Faire un clear du set
		stream_set_clear(&sset);
		//on remet tous les flux dans le set
		for (int i = 0; i < 3; ++i) {
			stream_set_add(&sset, &req_str[i]);
		}
	}
	//on close et on unlink tous les flux
	for (int i = 0; i < 3; ++i) {
		if (stream_close(&req_str[i]) < 0) {
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
		if (stream_unlink(&req_str[i], request_name) < 0) { 
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
	}	
	// On close le stream_manager
	manager_close();
	// On close le request_manager
	request_manager_close();
}

