/**
 * Définie les opération du manager de flux.
 * 
 */

#ifndef STREAM_MANAGER_H
#define STREAM_MANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stream.h"

/**
 * Initialise les flux pouvant être utilisés.
 */
void manager_init();

/**
 * Renvoie un flux de type "streamname".
 */
stream_t manager_getstream(const char* streamName);

/**
 * Ajoute le flux 'stream' de nom 'name' à la liste des flux créés
 *   par le processus
 */
void manager_addstream(stream_t* stream, const char* name);

/**
 * Retire le flux 'stream' de la liste des flux créés par le processus
 */
void manager_removestream(stream_t* stream);

/**
 * Supprime tous les flux de la liste et les retire de la liste
 */
void manager_clean();

/**
 * Nettoie les type de flux chargés.
 */
void manager_close();

#endif
