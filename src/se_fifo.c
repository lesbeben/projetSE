#include "se_fifo.h"

const char* _fifo_question = "";
const char* _fifo_answer = "";

streamd_t* _fifo_open(const char* name, int oflag, mode_t mode) {
	
	//A FAIRE
	
	return NULL;
}

int _fifo_close(streamd_t * sd) {
	
	//A FAIRE
	
	return 0;
}

int _fifo_read(streamd_t* fd, char* buffer, size_t size) {
	
	//A FAIRE
	
	return 0;
}

int _fifo_write(streamd_t* fd, char* buffer, size_t size) {
	
	//A FAIRE
	
	return 0;
}

const operation_t _fifo_op  = {
	_fifo_open, _fifo_close, _fifo_read, _fifo_write, "fif"
};
	
operation_t fifo_getOp() {
	return _fifo_op;
}
