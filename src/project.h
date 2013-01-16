/**
 * Définie les données et opérations communes au server et au client.
 */
 
#ifndef PROJECT_h
#define PROJECT_h


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define STRSIZ 512

/**
 * Renvoie le nom standard du server.
 */
const char* getRequestName();

/**
 * Renvoie le préfixe du nom standard des flux de réponse.
 * Le nom d'un flux de réponse sera de la forme : 
 *   "getAnswerPrefix() + pid du client"
 */
const char* getAnswerPrefix();

#endif
