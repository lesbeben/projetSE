#include "se_mq.h"

const char* _mq_question = "";
const char* _mq_answer = "";

streamd_t* _mq_open(const char* name, int oflag, mode_t mode) {
	
	//A FAIRE
	
	return NULL;
}

int _mq_close(streamd_t * sd) {
	
	//A FAIRE
	
	return 0;
}

int _mq_read(streamd_t* fd, char* buffer, size_t size) {
	
	//A FAIRE
	
	return 0;
}

int _mq_write(streamd_t* fd, char* buffer, size_t size) {
	
	//A FAIRE
	
	return 0;
}

const operation_t _mq_op = {
	_mq_open, _mq_close, _mq_read, _mq_write, "mq"
};
	
operation_t mq_getOp() {
	return _mq_op;
}
