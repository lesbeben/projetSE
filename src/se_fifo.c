#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "se_fifo.h"
#include "error.h"

/**
 * 
 */
static int _fifo_create(streamd_t* sd, const char* name, size_t size) {
	return mkfifo(name, S_IRUSR | S_IWUSR);
}

/**
 * 
 */
static int _fifo_open(streamd_t* sd, const char* name, int oflag) {
	int length = 2;
	if ((oflag & O_WRONLY) == 0) {
		length = 3;
	}
	sd->data = malloc(length * sizeof(int));
	int* datalength = (int*) sd->data;
	*datalength = length;
	int* fd = datalength + 1;
    *fd = open(name, oflag);
    
	if (!(oflag & O_WRONLY)) {
		int* fdout = fd + 1;
		// On ouvre le tube nommé en écriture pour rendre la lecture bloquante
		*fdout = open(name, O_WRONLY);
        if (*fdout == -1) {
			return -1;
		}
    }
	return *fd;
}

/**
 * Ferme le tube nommé
 */
static int _fifo_close(streamd_t * sd) {
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
	
	return res;
}

/**
 * 
 */
static int _fifo_read(streamd_t* sd, void* buffer, size_t size) {
    int* datalength = (int*) sd->data;
	int* fd = datalength + 1;
	return read(*fd, buffer, size);
}

/**
 * 
 */
static int _fifo_write(streamd_t* sd, void* buffer, size_t size) {    
    int* datalength = (int*) sd->data;
	int* fd = datalength + 1;
	return write(*fd, buffer, size);
}

/**
 * 
 */
static int _fifo_unlink(const char* name) {
	return unlink(name);
}

/*
 *
 */
static const operation_t _fifo_op  = {
	_fifo_create, _fifo_open, _fifo_close
	, _fifo_read, _fifo_write, _fifo_unlink, "fif"
};
	
operation_t fifo_getOp() {
	return _fifo_op;
}
