#include "se_fifo.h"
#include "error.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

const char* _fifo_question = "ProjetFifoQuestion";
const char* _fifo_answer = "ProjetFifoAnswer";

streamd_t* _fifo_open(const char* name, int oflag, mode_t mode, size_t size) {
    check_error((name == NULL), "_fifo_open : name == NULL\n");
    check_error((size <= 0),    "_fifo_open : size <= 0\n"   );
    
	streamd_t* sd = malloc(sizeof(streamd_t*));
    if (oflag & O_CREAT) {
        check_error2(mkfifo(name, mode) == -1, "mkfifo");
    }

    sd->sd.fd = open(name, oflag);
    check_error2((sd->sd.fd == -1), "open");
    
    if (mode & O_RDONLY) {
        // On ouvre le tube nommé en écriture pour rendre la lecture bloquante
        check_error2(open(name, O_WRONLY) == -1, "open");
    }

	return sd;
}

int _fifo_close(streamd_t * sd) {
    check_error((sd == NULL), "_fifo_close : sd == NULL\n");
	int fd = sd->sd.fd;
    free(sd);
	return close(fd);
}

int _fifo_read(streamd_t* fd, char* buffer, size_t size) {
	check_error((fd == NULL),     "_fifo_read : fd == NULL\n"    );
    check_error((buffer == NULL), "_fifo_read : buffer == NULL\n");
    check_error((size <= 0),      "_fifo_read : size <= 0\n"     );
    	
	return read(fd->sd.fd, buffer, size);
}

int _fifo_write(streamd_t* fd, char* buffer, size_t size) {
	check_error((fd == NULL),     "_fifo_write : fd == NULL\n"    );
    check_error((buffer == NULL), "_fifo_write : buffer == NULL\n");
    check_error((size <= 0),      "_fifo_write : size <= 0\n"     );
	//A FAIRE
	
	return write(fd->sd.fd, buffer, size);
}

/*
 *
 */
const operation_t _fifo_op  = {
	_fifo_open, _fifo_close, _fifo_read, _fifo_write, "fif"
};
	
operation_t fifo_getOp() {
	return _fifo_op;
}
