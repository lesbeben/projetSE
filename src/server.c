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
	//sigaction
	stream_t req_str = manager_getstream(); //a completer
	//3 getstream (tableau) en dur pour l'instant
	//un set avec les 3
	int n = 0;
	request_t req;

	//creer map (clientpid, tube)

	//stream create (+ stream open!)


	while ((n = stream_read(&req_str, &req, sizeof(request_t))) > 0) { //sizeof -> buffersize  read -> select > 0 || errno = INTR
	//for tous les types de stream (isset?)
	//si client pid pas dans map
		//creer tube
		//ajouter pid,tube a la map
		switch(fork()) {
			case -1:
				//envoyer message (signal)
				// faire une constante?
			case 0:
				//while read tube
				process_request(&req);
				//close tube
				exit(EXIT_SUCCESS); // -> raise
				//reprendre algo td
			default:
				//ecrire tube
				//close tube
				//retour
		}
	}
	//close
	//unlink
	
}
