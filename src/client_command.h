/**
 * Définie la structure de données d'une commande pour le client.
 */

#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include "request.h"

/**
 * Une commande du client est composée de : 
 *   - cmd_name : Le nom de la commande (3 lettres max)
 *   - request_cmd_func : la fonction de traitement de la commande
 *                          reçoie une commande et renvoie une requete.
 *   - help : Un fonction qui affiche l'aide de la commande.
 */
typedef struct {
	char cmd_name[4];
	request_t* (*request_cmd_func) (const char*);
    void (*help) ();
} request_cmd_t;

#endif
