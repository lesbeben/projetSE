#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "stream_manager.h"
#include "project.h"

//tester les appels de stream_* ?

void help(){
	printf("./server uid | gid | pid\n"); // a changer
}

void quit(){
	//peut etre factoriser ici l'envoie de signaux
}

void process_request(request_t req) {
    char ans_name[STRSIZ];
    sprintf(ans_name, "%s%i", answer_prefix, req.clientpid);
    stream_t req_str = manager_getstream(argv[argc-1]);
    stream_open(&req_str, ans_name, )

}

int main(int argc, char **argv) {

	if (argc != 2){
		help();
		exit(EXIT_FAILURE);
	}

	stream_t req_str = manager_getstream(argv[argc-1]);
	int n = 0;
	request_t req;



	//stream create (+ stream open?)


	while ((n = stream_read(&req_str, &req, sizeof(request_t))) > 0) {
		switch(fork()) {
			case -1:
				//envoyer message (signal)
				check_error2(1, "fork"); // faire une constante
			case 0:
				process_request(req);
				exit(EXIT_SUCCESS);
            //reprendre algo td




}
