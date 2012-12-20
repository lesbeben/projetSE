#include "se_shm.h"

const char* _shm_question = "";
const char* _shm_answer = "";

streamd_t* _shm_open(const char* name, int oflag, mode_t mode) {
	
	//A FAIRE
	
	return NULL;
}

int _shm_close(streamd_t * sd) {
	
	//A FAIRE
	
	return 0;
}

int _shm_read(streamd_t* fd, char* buffer, size_t size) {
	
	//A FAIRE
	
	return 0;
}

int _shm_write(streamd_t* fd, char* buffer, size_t size) {
	
	//A FAIRE
	
	return 0;
}

const operation_t _shm_op  = {
	_shm_open, _shm_close, _shm_read, _shm_write, "shm"
};

operation_t shm_getOp() {
	return _shm_op;
}
