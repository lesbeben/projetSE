#ifndef REQUEST_H
#define REQUEST_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Structure représentant une requete
 * la requete contiendra : 
 *   - un entier size représentant la taille totale de la requete
 *   - le pid du client envoyant la requete
 *   - le nom du type de flux à utiliser pour répondre à la requete
 *   - le nom de la commande
 *   - un champs data contenant les données de la commande
 */
typedef struct {
	size_t size;
	pid_t clientpid;
	char answer_stream[4];
	char cmdname[4];
	char data[1];
} request_t;

/**
 * Alloue une requete en fonction de la commande cmd
 * La requete doit etre détruite pour éviter les fuite de mémoires
 */
request_t* create_request(const char* cmd);

/**
 * Detruit une requete
 */
void delete_request(request_t* req);

/**
 * Remplie un buffer de la réponse à une requete
 */
int getanswer(request_t* req, char* buffer, size_t buffsize);

#endif
