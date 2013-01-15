/**
 * Définit les données et opérations communes au server et au client.
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

const char* getRequestName();

const char* getAnswerPrefix();

#endif
