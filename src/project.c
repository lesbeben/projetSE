/**
 * Implémente les fonctions et données communes au server et au client.
 * Membres publiques :
 *   - getRequestName()
 *   - getAnswerPrefix()
 * 
 * Membres privés :
 *   - request_name
 *   - answer_prefix
 */

#include "project.h"

static const char* request_name = "request_stream";
static const char* answer_prefix = "answer_stream";


const char* getRequestName() {
	return request_name;
}

const char* getAnswerPrefix() {
	return answer_prefix;
}
