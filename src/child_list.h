#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include <unistd.h>

/**
 * Ajoute un pipe pour le processus de traitement d'un client.
 */
void addChild(pid_t clientPid, int pipe);

/**
 * Renvoie 1 si un processus de traitement existe pour un pid.
 * Renvoie 0 autrement.
 */
int childExist(pid_t clientPid);

/**
 * Renvoie le pipe associé à un client.
 * renvoie -1 si le client n'existe pas;
 */
int getPipe(pid_t clientPid);

/**
 * Enleve un processus de traitement la liste des processus.
 * Peut etre utilisé dans un signal.
 */
void removeChild(pid_t clientPid);

/**
 * Enleve tous les processus de traitement de la liste.
 */
void childsClean();

#endif
