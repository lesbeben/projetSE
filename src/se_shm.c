#include "se_shm.h"
#include "error.h"


/**
 *
 */
int _shm_create(streamd_t* sd, const char* name, size_t size) {
	//A FAIRE
	return 0;
}

/**
 * Retourne un descripteur de flux sur un segment de mémoire partagé
 * Les paramètres sont les mêmes que la fonction 'shm_open'
 */
int _shm_open(streamd_t* sd, const char* name, int oflag) {
    //A FAIRE
	return 0;
}

/**
 * Ferme le segment de mémoire partagé
 */
int _shm_close(streamd_t * sd) {
    //A FAIRE
	return 0;
}

/**
 * Lit un maximum de size octets dans sd et les stockent dans buffer
 * Retourne le nombre d'octets lues, -1 si erreur
 * la fonction est bloquante
 */
int _shm_read(streamd_t* sd, void* buffer, size_t size) {
	//A FAIRE

	return 0;
}

/**
 * Ecrit size octets de buffer dans sd
 * Retourne le nombre d'octets écrits
 */
int _shm_write(streamd_t* sd, void* buffer, size_t size) {
	//A FAIRE

	return 0;
}

/**
 *
 */
int _shm_unlink(const char* name) {
    //A FAIRE
	return 0;
}

/*
 *
 */
const operation_t _shm_op  = {
	_shm_create, _shm_open, _shm_close
	, _shm_read, _shm_write, _shm_unlink, "shm"
};

operation_t shm_getOp() {
	return _shm_op;
}
