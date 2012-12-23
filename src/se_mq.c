#include "se_mq.h"
#include "error.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/**
 * 
 */
int _mq_create(streamd_t* sd, const char* name, size_t size) {
    struct mq_attr attr;
	attr.mq_msgsize = size;
	attr.mq_maxmsg = 10; 
    
    sd->data = malloc(sizeof(mqd_t*));
    mqd_t* mq = (mqd_t*) sd->data;
    char* realname = malloc(strlen(name) + 2);
    sprintf(realname, "/%s", name);
    *mq = mq_open(realname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    free(realname);
	return *mq;
}

/**
 * 
 */
int _mq_open(streamd_t* sd, const char* name, int oflag) {
    if (sd->data == NULL) {
		char* realname = malloc(strlen(name) + 2);
		sprintf(realname, "/%s", name);
		sd->data = malloc(sizeof(mqd_t*));
		mqd_t* mq = (mqd_t*) sd->data;
		*mq = mq_open(realname, oflag);
		free(realname);

		return *mq;
	}
    
	return 0;
}

/**
 * 
 */
int _mq_close(streamd_t* sd) {
    int res = 0;
    mqd_t* mq = sd->data;
    res = mq_close(*mq);
    free(sd->data);
    return res;
}

/**
 * 
 */
int _mq_read(streamd_t* sd, void* buffer, size_t size) {
    mqd_t* mq = (mqd_t*) sd->data;
	return mq_receive(*mq, buffer, size, NULL);
}

/**
 * 
 */
int _mq_write(streamd_t* sd, void* buffer, size_t size) {
    mqd_t* mq = sd->data;
	return mq_send(*mq, buffer, size, 0);
}

/**
 * 
 */
int _mq_unlink(const char* name) {
	return mq_unlink(name);
}

/*
 *
 */
const operation_t _mq_op = {
	_mq_create,_mq_open, _mq_close
	, _mq_read, _mq_write, _mq_unlink, "mq"
};

operation_t mq_getOp() {
	return _mq_op;
}
