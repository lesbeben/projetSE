#ifndef SE_SHM_H
#define SE_SHM_H

#include "stream.h"

/**
 * Retourne un descripteur de flux sur un segment de mémoire partagé
 * Les paramètres sont les mêmes que la fonction 'shm_open'
 */
streamd_t* _shm_open(const char* name, int oflag, mode_t mode, size_t size);

/**
 * Ferme le segment de mémoire partagé
 */
int _shm_close(streamd_t* sd);

/**
 * Lit un maximum de size octets dans sd et les stockent dans buffer
 * Retourne le nombre d'octets lues, -1 si erreur
 * la fonction est bloquante
 */
int _shm_read(streamd_t* sd, char* buffer, size_t size);

/**
 * Ecrit size octets de buffer dans sd
 * Retourne le nombre d'octets écrits
 */
int _shm_write(streamd_t* sd, char* buffer, size_t size);

/**
 */
operation_t shm_getOp();

#endif
