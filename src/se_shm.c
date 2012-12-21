#include "se_shm.h"
#include "error.h"


/**
 * Retourne un descripteur de flux sur un segment de mémoire partagé
 * Les paramètres sont les mêmes que la fonction 'shm_open'
 */
streamd_t* _shm_open(const char* name, int oflag, mode_t mode, size_t size) {
    check_error((name == NULL), "_shm_open : name == NULL\n");
    check_error((size <= 0),    "_shm_open : size <= 0\n"   );
	//A FAIRE
	
	return NULL;
}

/**
 * Ferme le segment de mémoire partagé
 */
int _shm_close(streamd_t * sd) {
	check_error((sd == NULL), "_shm_close : sd == NULL\n");
	//A FAIRE
	
	return 0;
}

/**
 * Lit un maximum de size octets dans sd et les stockent dans buffer
 * Retourne le nombre d'octets lues, -1 si erreur
 * la fonction est bloquante
 */
int _shm_read(streamd_t* sd, char* buffer, size_t size) {
	check_error((sd == NULL),     "_shm_read : fd == NULL\n"    );
    check_error((buffer == NULL), "_shm_read : buffer == NULL\n");
    check_error((size <= 0),      "_shm_read : size <= 0\n"     );
    
	//A FAIRE
	
	return 0;
}

/**
 * Ecrit size octets de buffer dans sd
 * Retourne le nombre d'octets écrits
 */
int _shm_write(streamd_t* sd, char* buffer, size_t size) {
	check_error((sd == NULL),     "_shm_write : fd == NULL\n"    );
    check_error((buffer == NULL), "_shm_write : buffer == NULL\n");
    check_error((size <= 0),      "_shm_write : size <= 0\n"     );
	//A FAIRE
	
	return 0;
}

/**
 * 
 */
int _shm_unlink(const char* name) {
	check_error((name == NULL), "_fshm_unlink : name == NULL\n");
	return unlink(name);
}

/*
 *
 */
const operation_t _shm_op  = {
	_shm_open, _shm_close, _shm_read, _shm_write, _shm_unlink, "shm"
};

operation_t shm_getOp() {
	return _shm_op;
}
