#include "se_mq.h"
#include "error.h"

const char* _mq_question = "/ProjetMqQuestion";
const char* _mq_answer = "/ProjetMqAnswer";

streamd_t* _mq_open(const char* name, int oflag, mode_t mode, size_t size) {
    check_error((name == NULL), "_mq_open : name == NULL\n");
    check_error((size <= 0),    "_mq_open : size <= 0\n"   );
    
    struct mq_attr attr;
	attr.mq_msgsize = size;
	attr.mq_maxmsg = 10;
    
    streamd_t* sd = malloc(sizeof(streamd_t*));
    sd->sd.mq = mq_open(name, oflag, mode, &attr);
    check_error2((sd->sd.mq == -1), "mq_open");
    
	return sd;
}

int _mq_close(streamd_t* sd) {
    check_error((sd == NULL), "_mq_close : sd == NULL\n");
    mqd_t mq = sd->sd.mq;
    free(sd);
    return mq_close(mq);
}

int _mq_read(streamd_t* fd, char* buffer, size_t size) {
	check_error((fd == NULL),     "_mq_read : fd == NULL\n"    );
    check_error((buffer == NULL), "_mq_read : buffer == NULL\n");
    check_error((size <= 0),      "_mq_read : size <= 0\n"     );
    
	return mq_receive(fd->sd.mq, buffer, size, NULL);
}

int _mq_write(streamd_t* fd, char* buffer, size_t size) {
	check_error((fd == NULL),     "_mq_write : fd == NULL\n"    );
    check_error((buffer == NULL), "_mq_write : buffer == NULL\n");
    check_error((size <= 0),      "_mq_write : size <= 0\n"     );
    
	return mq_send(fd->sd.mq, buffer, size, 0);
}

/*
 *
 */
const operation_t _mq_op = {
	_mq_open, _mq_close, _mq_read, _mq_write, "mq"
};

operation_t mq_getOp() {
	return _mq_op;
}
