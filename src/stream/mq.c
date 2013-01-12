#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "stream.h"
#include "../error.h"

/*
 * 
 */
static int _mq_create(streamd_t* sd, const char* name, size_t size) {
	int realnamelength = strlen(name) + 2;
	char realname[realnamelength];
    sprintf(realname, "/%s", name);
    
    struct mq_attr attr;
	attr.mq_msgsize = size;
	attr.mq_maxmsg = 10; 
    
    sd->data = malloc(sizeof(mqd_t*));
    mqd_t* mq = (mqd_t*) sd->data;
    *mq = mq_open(realname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attr);
    if (*mq == -1) {
		perror("mq_open");
		return -1;
	}

	return *mq;
}

/*
 * 
 */
static int _mq_open(streamd_t* sd, const char* name, int oflag) {
    if (sd->data == NULL) {
		int realnamelength = strlen(name) + 2;
		char realname[realnamelength];
		sprintf(realname, "/%s", name);
		sd->data = malloc(sizeof(mqd_t*));
		mqd_t* mq = (mqd_t*) sd->data;
		*mq = mq_open(realname, oflag);
		if (*mq == -1) {
			perror("mq_open");
			return -1;
		}
		return *mq;
	}
    
	return 0;
}

/*
 * 
 */
static int _mq_close(streamd_t* sd) {
    mqd_t* mq = sd->data;
    if (mq_close(*mq) == -1) {
		perror("mq_close");
		return -1;
	}
    free(sd->data);
    sd->data = NULL;
    
    return 0;
}

/*
 * 
 */
static int _mq_read(streamd_t* sd, void* buffer, size_t size) {
    mqd_t* mq = (mqd_t*) sd->data;
    int n = mq_receive(*mq, buffer, size, NULL);
    if (n == -1) {
		perror("mq_receive");
		return -1;
	}
	return n;
}

/*
 * 
 */
static int _mq_write(streamd_t* sd, void* buffer, size_t size) {
    mqd_t* mq = sd->data;
	int n = mq_send(*mq, buffer, size, 0);
    if (n == -1) {
		perror("mq_send");
		return -1;
	}
	return n;
}

/*
 * 
 */
static int _mq_getfd(streamd_t* sd) {
	if (sd->data == NULL) {
		fprintf(stderr, "_mq_getfd : Le flux n'est pas ouvert\n");
		return -2;
	}
	return *((int*) sd->data);
}

/*
 * 
 */
static int _mq_unlink(const char* name) {
	int realnamelength = strlen(name) + 2;
	char realname[realnamelength];
    sprintf(realname, "/%s", name);
    
    if (mq_unlink(realname) == -1) {
		perror("mq_unlink");
		return -1;
	}
	return 0;
}

/*
 *
 */
const operation_t _mq_op = {
	_mq_create,_mq_open, _mq_close
	, _mq_read, _mq_write, _mq_getfd, _mq_unlink, "mq"
};

operation_t getop() {
	return _mq_op;
}
