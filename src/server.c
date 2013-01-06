#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "stream_manager.h"
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
	getanswer(req, &buf, BUFSIZ -1); // ou un switch sur les cmdname?
	//on envoie la reponse
	if (stream_write(&ans_str, &buf, BUFSIZE) < 0) {
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
	printf("recu signal: %d", signum);
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
	
	//on cree en dur les types de stream
	stream_t req_str[3]//faire des constantes de type TUBE SHM MQ
	stream_set_t sset;
	req_str[0] = manager_getstream('fif');
	req_str[1] = manager_getstream('mq');
	req_str[2] = manager_getstream('shm');
	int n = 0;
	request_t req;
	
	//creer map (clientpid, tube)

	//on cree et ouvre les trois streams et on les ajoute au set
	for (int i = 0; i < 3; ++i) {
		if (stream_create(req_str[i], request_name, BUFSIZ) < 0) { //quelle taille pour le create?
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
		if (stream_open(req_str[i], request_name, O_RDONLY) < 0) { 
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
		stream_set_add(&sset, req_str[i]);
	}

	//on lance le serveur
	while ((n = stream_set_select(&sset)) > 0) {
		//pour tous les types de flux qui sont actifs
		for (int i = 0; i < 3; ++i) {
			if ((n =stream_set_isset(&sset, &req_str[i]) > 0)) { //on devrait l'appeler stream_set_isinset ou inset plutot non?
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
							while(((n = read(tube[0], req, BUFSIZ)) > 0) && (errno != EINTR)){
								process_request(&req);
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
							if (stream_read(&req_str[i], &req, BUFSIZE) < 0) {
								raise(SIGTERM);
								//plantage
							}
							
							//on ecrit dans le tube
							if (write(tube[1], &req, BUFSIZ) < 0) {
								perror("write");
								raise(SIGTERM);
								//plantage
							}
							
							//on ferme l'entree du tube ? on garde pour pouvoir reutiliser au prochain tour?
							//close(tube[1]);
							
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
		//on remet tous les flux dans le set
		for (int i = 0; i < 3; ++i) {
			stream_set_add(&sset, req_str[i]);
		}
	}
	//on close et on unlink tous les flux
	for (int i = 0; i < 3; ++i) {
		if (stream_close(&req_str[i]) < 0) {
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
		if (stream_unlink(&req_str[i]) < 0) { 
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
	}
}

