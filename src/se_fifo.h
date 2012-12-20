#ifndef SE_FIFO_H
#define SE_FIFO_H

#include "stream.h"

/**
 * Retourne un descripteur sur un tube nommé
 */
streamd_t* _fifo_open(const char* name, int oflag, mode_t mode, size_t size);

/**
 * Ferme le tube nommé
 */
int _fifo_close(streamd_t * sd);

/**
 * Lit un maximum de size octets dans fd et les stockent dans buffer
 * Retourne le nombre d'octets lues
 */
int _fifo_read(streamd_t* fd, char* buffer, size_t size);

/**
 * Ecrit size octets de buffer dans fd
 * Retourne le nombre d'octets écrits
 */
int _fifo_write(streamd_t* fd, char* buffer, size_t size);


/**
 */
operation_t fifo_getOp();

	
#endif
