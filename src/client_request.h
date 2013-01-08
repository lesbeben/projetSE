#ifndef CLIENT_REQUEST_H
#define CLIENT_REQUEST_H

#include "request.h"

/**
 * Initialise les commandes de traitement des requetes.
 */
void request_manager_init();

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
 * Affiche l'aide de toute les commandes.
 */
void request_manager_helpAll();

/**
 * Affiche l'aide de la commande d'une commande.
 */
void request_manager_help(const char* cmd);
 
/**
 * Ferme les commandes de traitement des requetes.
 */
void request_manager_close();

#endif
