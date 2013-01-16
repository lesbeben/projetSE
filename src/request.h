/**
 * Définie la structure de données d'une requete.
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Structure représentant une requete.
 * La requete contiendra : 
 *   - un entier size représentant la taille totale de la requete
 *   - le pid du client envoyant la requete
 *   - le nom de la commande
 *   - un champs data contenant les données de la commande
 */
typedef struct {
	size_t size;
	pid_t clientpid;
	char cmdname[4];
	char data[1];
} request_t;

#endif
