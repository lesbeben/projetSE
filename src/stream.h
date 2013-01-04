#ifndef STREAM_H
#define STREAM_H

#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>

/**
 *
 */
typedef struct {
    void* data;
} streamd_t;

/**
 *
 */
typedef struct {
	int (*_create) (streamd_t*, const char*, size_t);
    int (*_open) (streamd_t*, const char*, int);
    int (*_close) (streamd_t*);
    int (*_read) (streamd_t*, void*, size_t);
    int (*_write) (streamd_t*, void*, size_t);
    int (*_getfd) (streamd_t*);
    int (*_unlink) (const char*);
    char name[4];
} operation_t;

/**
 *
 */
typedef struct {
	operation_t op;
    streamd_t sd;
} stream_t;

/**
 * Crée le flux de données.
 * Parametre
 *   - size : Taille pour laquelle les données sont envoyées en totalité
 *   - name : Nom du flux à créer
 * Pre-condition
 *   - size > 0
 *   - name != NULL
 *   - name est un nom de flux valide
 *   - stream != NULL
 * Renvoie 
 *    >= 0 en cas de réussite
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_create(stream_t* stream, const char* name, size_t size);

/**
 * Ouvre le flux de données
 * Parametre
 *   - name : Nom du flux à ouvrir
 *   - oflag : flag d'ouverture du flux
 * Pre-condition
 *   - stream != NULL
 *   - name != NULL
 *   - name est un nom de flux valide
 *   - oflag == O_RDONLY xor O_WDONLY
 * Renvoie
 *   >= 0 en cas de réussite
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_open(stream_t* stream, const char* name, int oflag);

/**
 * Ferme le flux de données
 * Parametre
 *   - stream : Le flux à fermer
 * Pre-condition
 *   - stream != NULL
 *   - stream est ouvert
 * Renvoie
 *   >= 0 en cas de réussite
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_close(stream_t* stream);

/**
 * Lit un maximum de size octets dans le flux et les stockent dans buffer
 * Parametre
 *   - stream : Le flux dans lequel lire
 *   - buffer : Le buffer dans lequel écrire les données lues
 *   - size : La taille maximale de données à lire
 * Pre-condition
 *   - stream != NULL
 *   - stream est ouvert
 *   - buffer != NULL
 *   - size > 0
 * Renvoie
 *   Le nombre d'octets lues en cas de réussite 
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_read(stream_t* stream, void* buffer, size_t size);

/**
 * Ecrit size octets de buffer dans le flux
 * Parametre
 *   - stream : Le flux dans lequel écrire
 *   - buffer : Le buffer dans lequel lire les données
 *   - size : La taille maximale de données à écrire
 * Pre-condition
 *   - stream != NULL
 *   - stream est ouvert
 *   - buffer != NULL
 *   - size > 0
 * Renvoie
 *   Le nombre d'octets écrit en cas de réussite 
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_write(stream_t* stream, void* buffer, size_t size);

/**
 * Unlink le flux 'name'
 * Parametre
 *   - stream : Le flux à unlink
 *   - name : Le nom du flux à unlink
 * Pre-condition
 *   - stream != NULL
 *   - name != NULL
 *   - name est un nom de flux valide
 * Renvoie
 *   >= 0 en cas de réussite
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_unlink(stream_t* stream, const char* name);

#endif
