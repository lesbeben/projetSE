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
#include "stream_manager.h"
#include "project.h" //verifier les includes



int main(int argc, char **argv) {
	request_t *req = NULL;
	char ans_buf[BUFSIZ];
	char cmd_buf[BUFSIZ];
	//sigaction a creer
	
	int res= 0
	while (res != -233) //code exit
		//on lit la commande sur l'entree standard
		scanf("%s\n", &cmd_buf);
		//on cree la requete
		req = create_request(cmdbuf);
		//on envoie la question
		res = getanswer(req, &buf, BUFSIZ -1);
		//on efface la requete
		delete_request(req);
	//close ???
	//unlink???
	return EXIT_SUCCESS;
}
