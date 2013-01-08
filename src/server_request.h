#ifndef SERVER_REQUEST_H
#define SERVER_REQUEST_H

#include "request.h"

/**
 * Initialise les commandes de traitement des requetes.
 */
void request_manager_init();

/**
 * Remplie un buffer avec la réponse à une requete
 */
int get_answer(request_t* req, char* buffer, size_t buffsize);

/**
 * Ferme les commandes de traitement des requetes.
 */
void request_manager_close();

#endif
