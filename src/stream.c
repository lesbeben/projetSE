#include "stream.h"
#include "error.h"

void stream_open(stream_t* stream, const char* name, int oflag, mode_t mode, size_t size) {
    check_error((stream == NULL), "stream_open : stream == NULL\n");
    check_error((name == NULL),   "stream_open : name == NULL\n"  );
    check_error((size <= 0),      "stream_open : size <= 0\n"     );
    stream->sd = stream->op->_open(name, oflag, mode, size);
}

int stream_close(stream_t* stream) {
    check_error((stream == NULL),     "stream_close : stream == NULL\n"    );
    check_error((stream->sd == NULL), "stream_close : stream->sd == NULL\n");
	return stream->op->_close(stream->sd);
}

int stream_read(stream_t* stream, char* buffer, size_t size) {
    check_error((stream == NULL),     "stream_read : stream == NULL\n"    );
    check_error((stream->sd == NULL), "stream_read : stream->sd == NULL\n");
    check_error((buffer == NULL),     "stream_read : buffer == NULL\n"    );
    check_error((size <= 0),          "stream_read : size <= 0\n"         );
	return stream->op->_write(stream->sd, buffer, size);
}

int stream_write(stream_t* stream, char* buffer, size_t size) {
    check_error((stream == NULL),     "stream_write : stream == NULL\n"    );
    check_error((stream->sd == NULL), "stream_write : stream->sd == NULL\n");
    check_error((buffer == NULL),     "stream_write : buffer == NULL\n"    );
    check_error((size <= 0),          "stream_write : size <= 0\n"         );
	return stream->op->_read(stream->sd, buffer, size);
}
