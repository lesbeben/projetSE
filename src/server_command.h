/**
 * Définie la structure de données utilisée pour les commandes du server.
 */

#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H

#include "request.h"

/**
 * Une commande du server est composée de : 
 *   - cmd_name : Le nom de la commande (3 lettres max)
 *   - answer_cmd_func : la fonction de traitement de la commande
 */
typedef struct {
	char cmd_name[4];
	int (*answer_cmd_func) (request_t*, char*, size_t);
} answer_cmd_t;

#endif
