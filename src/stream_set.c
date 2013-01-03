#include "stream_set.h"

int stream_set_add(stream_set_t* set, stream_t* stream) {
	if (set == NULL) {
		fprintf(stderr, "stream_set_add : set == NULL\n");
		return -2;
	}
	if (stream == NULL) {
		fprintf(stderr, "stream_set_add : stream == NULL\n");
		return -2;
	}
	int fd = stream->op._getfd(&stream->sd);
	if (fd > set->_maxfd) {
		set->_maxfd = fd;
	}
	FD_SET(fd, &set->_set);
	return 0;
}

int stream_set_clear(stream_set_t* set) {
	if (set == NULL) {
		fprintf(stderr, "stream_set_clear : set == NULL\n");
		return -2;
	}
	set->_maxfd = 0;
	FD_ZERO(&set->_set);
	return 0;
}

int stream_set_isset(stream_set_t* set, stream_t* stream) {
	if (set == NULL) {
		fprintf(stderr, "stream_set_isset : set == NULL\n");
		return -2;
	}
	if (stream == NULL) {
		fprintf(stderr, "stream_set_isset : stream == NULL\n");
		return -2;
	}
	return FD_ISSET(stream->op._getfd(&stream->sd), &set->_set);
}

int stream_set_select(stream_set_t* set) {
	if (set == NULL) {
		fprintf(stderr, "stream_set_select : set == NULL\n");
		return -2;
	}
	return select(set->_maxfd + 1, &set->_set, NULL, NULL, NULL);
}
