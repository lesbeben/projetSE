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

/*
 * 
 */
static int _fifo_create(streamd_t* sd, const char* name, size_t size) {
	if (mkfifo(name, S_IRUSR | S_IWUSR) == -1) {
		perror("mkfifo");
		return -1;
	}
	return 0;
}

/*
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
    if (*fd == -1) {
		perror("open");
		return -1;
	}
	if (!(oflag & O_WRONLY)) {
		int* fdout = fd + 1;
		// On ouvre le tube nomm� en �criture pour rendre la lecture bloquante
		*fdout = open(name, O_WRONLY);
        if (*fdout == -1) {
			perror("open");
			return -1;
		}
    }
	return *fd;
}

/*
 *
 */
static int _fifo_close(streamd_t * sd) {
    int* datalength = sd->data;
	int* fd = datalength + 1;
	if (*datalength == 2) {
		int* fdout = fd + 1;
		if (close(*fdout) == -1) {
			perror("close");
			return -1;
		}
	}
	if (close(*fd) == -1) {
		perror("close");
		return -1;
	}
	free(sd->data);
	
	return 0;
}

/*
 * 
 */
static int _fifo_read(streamd_t* sd, void* buffer, size_t size) {
    int* datalength = (int*) sd->data;
	int* fd = datalength + 1;
	int n = read(*fd, buffer, size);
	if (n < 0) {
		perror("read");
		return -1;
	}
	return n;
}

/*
 * 
 */
static int _fifo_write(streamd_t* sd, void* buffer, size_t size) {    
    int* datalength = (int*) sd->data;
	int* fd = datalength + 1;
	int n = write(*fd, buffer, size);
	if (n < 0) {
		perror("write");
		return -1;
	}
	return n;
}

/*
 * 
 */
static int _fifo_unlink(const char* name) {
	if (unlink(name) == -1) {
		perror("unlink");
		return -1;
	}
	return 0;
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
