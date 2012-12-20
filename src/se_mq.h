#ifndef SE_MQ_H
#define SE_MQ_H

#include "stream.h"

/**
 * Retourne un descripteur sur une file de messages
 * Les paramètres sont les mêmes que la fonction 'mq_open'
 */
streamd_t* _mq_open(const char* name, int oflag, mode_t mode);

/**
 *  Ferme la file de messages
 */
int _mq_close(streamd_t* sd);

/**
 * Lit un maximum de size octets dans sd et les stockent dans buffer
 * Retourne le nombre d'octets lues
 */
int _mq_read(streamd_t* sd, char* buffer, size_t size);

/**
 * Ecrit size octets de buffer dans fd
 * Retourne le nombre d'octets écrits
 */
int _mq_write(streamd_t* fd, char* buffer, size_t size);

/**
 */
operation_t mq_getOp();

	
#endif
