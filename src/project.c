/**
 * Implémente les fonctions et données communes au server et au client.
 */

#include "project.h"

static const char* request_name = "request_stream";
/*
 * Préfixe du nom du tube nommé de réponse
 * Le nom du tube nommé sera de la forme :
 *   "answer + pid du client"
 */
static const char* answer_prefix = "answer_stream";


const char* getRequestName() {
	return request_name;
}

const char* getAnswerPrefix() {
	return answer_prefix;
}
