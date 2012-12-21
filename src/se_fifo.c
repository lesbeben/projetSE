#include "se_fifo.h"
#include "error.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * Retourne un descripteur sur un tube nommé
 */
streamd_t* _fifo_open(const char* name, int oflag, mode_t mode, size_t size) {
    check_error((name == NULL), "_fifo_open : name == NULL\n");
    check_error((size <= 0),    "_fifo_open : size <= 0\n"   );
    
	streamd_t* sd = malloc(sizeof(streamd_t*));
    if (oflag & O_CREAT) {
        check_error2(mkfifo(name, mode) == -1, "mkfifo");
    }
	int length = 1;
	if (mode & O_RDONLY) {
		length = 2;
	}
	sd->data = malloc(length * sizeof(int));
	int* datalength = (int*) sd->data;
	*datalength = length;
	int* fd = datalength + 1;
    *fd = open(name, oflag);
    check_error2((*fd == -1), "open");
    
    if (mode & O_RDONLY) {
		int* fdout = fd + 1;
		// On ouvre le tube nommé en écriture pour rendre la lecture bloquante
		*fdout = open(name, O_WRONLY);
        check_error2((*fdout == -1), "open");
    }

	return sd;
}

/**
 * Ferme le tube nommé
 */
int _fifo_close(streamd_t * sd) {
    check_error((sd == NULL), "_fifo_close : sd == NULL\n");
    int res = 0;
    int* datalength = sd->data;
	int* fd = datalength + 1;
	if (*datalength == 2) {
		int* fdout = fd + 1;
		res = close(*fdout);
	}
	if (close(*fd) == -1) {
		res = -1;
	}
	free(sd->data);
    free(sd);
	return res;
}

/**
 * Lit un maximum de size octets dans fd et les stockent dans buffer
 * Retourne le nombre d'octets lues
 */
int _fifo_read(streamd_t* sd, char* buffer, size_t size) {
	check_error((sd == NULL),     "_fifo_read : fd == NULL\n"    );
    check_error((buffer == NULL), "_fifo_read : buffer == NULL\n");
    check_error((size <= 0),      "_fifo_read : size <= 0\n"     );
    int* datalength = sd->data;
	int* fd = datalength + 1;
	return read(*fd, buffer, size);
}

/**
 * Ecrit size octets de buffer dans fd
 * Retourne le nombre d'octets écrits
 */
int _fifo_write(streamd_t* sd, char* buffer, size_t size) {
	check_error((sd == NULL),     "_fifo_write : fd == NULL\n"    );
    check_error((buffer == NULL), "_fifo_write : buffer == NULL\n");
    check_error((size <= 0),      "_fifo_write : size <= 0\n"     );
    
    int* datalength = sd->data;
	int* fd = datalength + 1;
	
	return write(*fd, buffer, size);
}

/**
 * 
 */
int _fifo_unlink(const char* name) {
	check_error((name == NULL), "_fifo_unlink : name == NULL\n");
	return unlink(name);
}

/*
 *
 */
const operation_t _fifo_op  = {
	_fifo_open, _fifo_close, _fifo_read, _fifo_write, _fifo_unlink, "fif"
};
	
operation_t fifo_getOp() {
	return _fifo_op;
}
