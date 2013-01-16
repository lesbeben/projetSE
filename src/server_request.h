/**
 * Définie les opérations de gestion des requetes pour le server.
 */
#ifndef SERVER_REQUEST_H
#define SERVER_REQUEST_H

#include "request.h"

/**
 * Initialise les commandes de traitement des requetes.
 */
void request_manager_init();

/**
 * Remplie un buffer avec la réponse à une requete.
 * Parametre : 
 *   - req : La requete à traiter
 *   - buffer : le buffer à remplir
 *   - buffsize : la taille max du buffer
 * Pre-condition :
 *   - req != NULL
 *   - buffer != NULL
 *   - taille du buffer >= buffsize
 * Renvoie
 *   >= 0 en cas de succes
 *   -1 en cas d'erreur
 *   -2 si le moyen de communication doit etre changé
 *      dans ce cas buffer contient le moyen de communication à utiliser.
 *   -3 Le client se déconnecte
 */
int get_answer(request_t* req, char* buffer, size_t buffsize);

/**
 * Ferme les commandes de traitement des requetes.
 */
void request_manager_close();

#endif
