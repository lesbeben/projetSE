#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "const.h"
#include "project.h" //c'est la même chose bordel!!

void help(){
	printf("Ca marche pas comme ça ducon\n"); // a changer
}

void quit(){
	//
	//
}


int main(int argc, char **argv) {
	
	if (argc != 2){
		help();
		exit(EXIT_FAILURE);
	}
	
	stream_t s = getStream(argv[argc-1]);
	int n = 0;
	request_t req;
	
	stream_open(&s, request_name, O_CREAT | O_WRONLY | O_TRUNC
				, S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH
				, sizeof(request_t));
	
	while ((n = stream_read(&s, &req, sizeof(request_t))) > 0) {
		switch(fork()) {
			case -1:
				//envoyer message (signal custom)
				check_error2(1, "fork"); // faire une constante
			case 0:
				process_request();
				exit(EXIT_SUCCESS);
				
	
	
	
		
}
