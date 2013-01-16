/**
 * Définie les opérations nécessaires pour gérer les processus 
 *   de traitement du server.
 */
#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include <unistd.h>

/**
 * Ajoute un pipe pour le processus de traitement d'un client.
 * Parametre
 *   - clientPid : Le client à ajouter à la liste des clients.
 *   - pipe : Le tube de communication associé au processus 
 *              de traitement du client clientPid
 * Pre-condition
 *   - pipe != -1
 */
void addChild(pid_t clientPid, int pipe);

/**
 * Renvoie 1 si un processus de traitement existe pour un pid.
 * Renvoie 0 autrement.
 * Parametre
 *   - clientPid : le pid du client dont on cherche si il 
 *                   existe un processus de traitement.
 */
int childExist(pid_t clientPid);

/**
 * Renvoie le pipe associé à un client.
 * Parametre
 *   - clientPid : le pid du client dont on cherche le tube.
 * Renvoie
 *   Le tube si le client existe 
 *   -1 si le client n'existe pas
 */
int getPipe(pid_t clientPid);

/**
 * Enleve un processus de traitement de la liste des processus.
 * Peut etre utilisé dans un signal.
 * Parametre
 *   - clientPid : le pid du client dont on supprime de la liste.
 */
void removeChild(pid_t clientPid);

/**
 * Enleve tous les processus de traitement de la liste.
 */
void childsClean();

#endif
