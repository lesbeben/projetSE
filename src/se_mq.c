#include "se_mq.h"
#include "error.h"

/**
 * Retourne un descripteur sur une file de messages
 * Les paramètres sont les mêmes que la fonction 'mq_open'
 */
streamd_t* _mq_open(const char* name, int oflag, mode_t mode, size_t size) {
    check_error((name == NULL), "_mq_open : name == NULL\n");
    check_error((size <= 0),    "_mq_open : size <= 0\n"   );
    
    struct mq_attr attr;
	attr.mq_msgsize = size;
	attr.mq_maxmsg = 10;
    
    streamd_t* sd = malloc(sizeof(streamd_t*));
    sd->data = malloc(sizeof(mqd_t*));
    mqd_t* mq = (mqd_t*) sd->data;
    *mq = mq_open(name, oflag, mode, &attr);
    check_error2((*mq == -1), "mq_open");
    
	return sd;
}

/**
 *  Ferme la file de messages
 */
int _mq_close(streamd_t* sd) {
    check_error((sd == NULL), "_mq_close : sd == NULL\n");
    int res = 0;
    mqd_t* mq = sd->data;
    res = mq_close(*mq);
    free(sd->data);
    free(sd);
    return res;
}

/**
 * Lit un maximum de size octets dans sd et les stockent dans buffer
 * Retourne le nombre d'octets lues
 */
int _mq_read(streamd_t* sd, char* buffer, size_t size) {
	check_error((sd == NULL),     "_mq_read : fd == NULL\n"    );
    check_error((buffer == NULL), "_mq_read : buffer == NULL\n");
    check_error((size <= 0),      "_mq_read : size <= 0\n"     );
    
	return mq_receive(sd->data, buffer, size, NULL);
}

/**
 * Ecrit size octets de buffer dans fd
 * Retourne le nombre d'octets écrits
 */
int _mq_write(streamd_t* sd, char* buffer, size_t size) {
	check_error((sd == NULL),     "_mq_write : fd == NULL\n"    );
    check_error((buffer == NULL), "_mq_write : buffer == NULL\n");
    check_error((size <= 0),      "_mq_write : size <= 0\n"     );
    
	return mq_send(sd->data, buffer, size, 0);
}

/**
 * 
 */
int _mq_unlink(const char* name) {
	check_error((name == NULL), "_mq_unlink : name == NULL\n");
	return mq_unlink(name);
}

/*
 *
 */
const operation_t _mq_op = {
	_mq_open, _mq_close, _mq_read, _mq_write, _mq_unlink, "mq"
};

operation_t mq_getOp() {
	return _mq_op;
}
