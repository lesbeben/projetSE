#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "stream_manager.h"
#include "project.h" //verifier les includes

//tester les appels de stream_*?  oui!!

void help(){
	printf("./server \n"); // a changer
}

void quit(){
	//peut etre factoriser ici l'envoie de signaux
}

void process_request(request_t *req) {
    char ans_name[STRSIZ];
    sprintf(ans_name, "%s%i", answer_prefix, req.clientpid);
    stream_t req_str = manager_getstream();//a completer avec le type de la req
    stream_open(&req_str, ans_name, )
    //envoyer
    //fermer

}

int main(int argc, char **argv) {

	if (argc != 2){ // ou pas
		help();
		exit(EXIT_FAILURE);
	}
	//sigaction a initialiser
	stream_t req_str[3]//faire des constantes de type TUBE SHM MQ
	stream_set_t sset;
	//on cree en dur les types de stream
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
			if (stream_close(req_str[i]) < 0) {
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
		if (stream_unlink(req_str[i]) < 0) { 
			//ajouter gestion des cas -1 -2
			raise(SIGTERM);
		}
		}
	
}

