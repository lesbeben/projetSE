/**
 * Définie les opérations de création des processus 
 *   de traitement du serveur.
 */

#ifndef CHILD_PROCESS_H
#define CHILD_PROCESS_H

/**
 * Créé un processus de traitement.
 * Parametre
 *   - pipe : Le tube de communication entre le server 
 *              et le processus de traitement.
 *   - clientPid : Le client à traiter dans le processus de traitement.
 */
void createChild(int pipe, pid_t clientPid);

#endif
