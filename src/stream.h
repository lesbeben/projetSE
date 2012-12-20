#ifndef STREAM_H
#define STREAM_H

#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>

/**
 * 
 */
typedef struct {
    union {
        int fd;
        mqd_t mq;
    } sd;
} streamd_t;

/**
 * 
 */
typedef struct {
    streamd_t*  (*_open) (const char*, int , mode_t, size_t size);
    int (*_close) (streamd_t* fd);
    int (*_read) (streamd_t*, char*, size_t);
    int (*_write) (streamd_t*, char*, size_t);
    char name[4];
} operation_t;


/**
 *
 */
typedef struct {
	operation_t* op;
    streamd_t* sd;
} stream_t;

/**
 * Ouvre le flux de données
 */
void stream_open(stream_t* stream, const char* name, int oflag, mode_t mode);

/**
 * Ferme le flux de données
 */
int stream_close(stream_t* stream);

/**
 * Lit un maximum de size octets dans le flux et les stockent dans buffer
 *  Retourne le nombre d'octets lues
 */
int stream_read(stream_t* stream, char* buffer, size_t size);

/**
 * Ecrit size octets de buffer dans le flux
 * Retourne le nombre d'octets écrits
 */
int stream_write(stream_t* stream, char* buffer, size_t size);

#endif
