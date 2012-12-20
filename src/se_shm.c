#include "se_shm.h"
#include "error.h"

const char* _shm_question = "/ProjetShmQuestion";
const char* _shm_answer = "/ProjetShmAnswer";

streamd_t* _shm_open(const char* name, int oflag, mode_t mode, size_t size) {
    check_error((name == NULL), "_shm_open : name == NULL\n");
    check_error((size <= 0),    "_shm_open : size <= 0\n"   );
	//A FAIRE
	
	return NULL;
}

int _shm_close(streamd_t * sd) {
	check_error((sd == NULL), "_shm_close : sd == NULL\n");
	//A FAIRE
	
	return 0;
}

int _shm_read(streamd_t* fd, char* buffer, size_t size) {
	check_error((fd == NULL),     "_shm_read : fd == NULL\n"    );
    check_error((buffer == NULL), "_shm_read : buffer == NULL\n");
    check_error((size <= 0),      "_shm_read : size <= 0\n"     );
    
	//A FAIRE
	
	return 0;
}

int _shm_write(streamd_t* fd, char* buffer, size_t size) {
	check_error((fd == NULL),     "_shm_write : fd == NULL\n"    );
    check_error((buffer == NULL), "_shm_write : buffer == NULL\n");
    check_error((size <= 0),      "_shm_write : size <= 0\n"     );
	//A FAIRE
	
	return 0;
}

/*
 *
 */
const operation_t _shm_op  = {
	_shm_open, _shm_close, _shm_read, _shm_write, "shm"
};

operation_t shm_getOp() {
	return _shm_op;
}
