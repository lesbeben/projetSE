#ifndef STREAM_SET_H
#define STREAM_SET_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include "stream.h"

typedef struct {
	fd_set _set;
	int _maxfd;
} stream_set_t;

/**
 * Ajoute un flux à un ensemble de flux.
 * Parametre
 *   - set : L'ensemble à modifier
 *   - stream : Le flux à ajouter
 * Pre-condition
 *   - set != NULL
 *   - stream != NULL
 *   - stream est ouvert
 * Renvoie
 *   >= 0 en cas de réussite
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_set_add(stream_set_t* set, stream_t* stream);

/**
 * Remet l'ensemble de flux à zero
 * Parametre
 *   - set : L'ensemble à modifier
 * Pre-condition
 *   - set != NULL
 * Renvoie
 *   >= 0 en cas de réussite
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_set_clear(stream_set_t* set);

/**
 * Teste si un flux est dans un ensemble de flux
 * Parametre
 *   - set : L'ensemble de flux à tester
 *   - stream : Le flux à chercher
 * Pre-condition
 *   - set != NULL
 *   - stream != NULL
 *   - stream est ouvert
 * Renvoie
 *   > 0 si le flux appartient à l'ensemble
 *   = 0 si le flux n'est pas dans l'ensemble
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_set_isset(stream_set_t* set, stream_t* stream);

/**
 * Bloque jusqu'à ce qu'un flux de l'ensemble soit disponible en lecture
 * Modifie l'ensemble pour que celui ci ne contienne
 *   que les flux disponibles
 * Parametre
 *   - set : L'ensemble de flux
 * Pre-condition
 *   - set != NULL
 * Renvoie
 *   Le nombre de flux disponibles en cas de succes
 *   -1 en cas d'erreur d'un appel systeme
 *   -2 en cas d'un argument invalide
 */
int stream_set_select(stream_set_t* set, struct timeval* timeout);

#endif
