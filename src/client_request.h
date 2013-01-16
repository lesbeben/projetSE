/**
 * Définie les opérations de traitement des requetes pour le client.
 */

#ifndef CLIENT_REQUEST_H
#define CLIENT_REQUEST_H

#include "request.h"

/**
 * Initialise les commandes de traitement des requetes.
 * Les commandes sont chargées dans : "plugin/clientcmd/"
 */
void request_manager_init();

/**
 * Alloue une requete en fonction de la commande cmd
 * La requete doit etre détruite pour éviter les fuite de mémoires
 * Parametre
 *   - cmd : La commande à traiter
 * Pre-condiiton
 *   - cmd != NULL
 * Renvoie
 *   La requete en cas de succes
 *   NULL en cas d'échec
 */
request_t* create_request(const char* cmd);

/**
 * Detruit une requete.
 * Parametre
 *   - req : La commande à détruire
 * Pre-condition
 *   - req != NULL
 */
void delete_request(request_t* req);

/**
 * Affiche l'aide de toute les commandes.
 */
void request_manager_helpAll();

/**
 * Affiche l'aide d'une commande.
 */
void request_manager_help(const char* cmd);
 
/**
 * Ferme les commandes de traitement des requetes.
 */
void request_manager_close();

#endif
