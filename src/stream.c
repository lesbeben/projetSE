#include "stream.h"
#include "error.h"
#include "stream_manager.h"

int stream_create(stream_t* stream, const char* name, size_t size) {
    int error = 0;
    error = error | check_error_noquit((stream == NULL)
        , "stream_create : stream == NULL\n");
    error = error | check_error_noquit((name == NULL)
        , "stream_create : name == NULL\n");
    error = error | check_error_noquit((size <= 0)
        , "stream_create : size <= 0\n");
    if (error){
		return -2;
	}
	manager_addstream(stream, name);
    return stream->op._create(&stream->sd, name, size);
}

int stream_open(stream_t* stream, const char* name, int oflag) {
    int flagcheck = (oflag == O_RDONLY) ^ (oflag == O_WRONLY);
    int error = 0;
    error = error | check_error_noquit((stream == NULL)
        , "stream_open : stream == NULL\n");
    error = error | check_error_noquit((name == NULL)
        , "stream_open : name == NULL\n");
    error = error | check_error_noquit(!flagcheck
        , "stream_open : oflag invalide\n");
    if (error) {
        return -2;
    }
    return stream->op._open(&(stream->sd), name, oflag);
}

int stream_close(stream_t* stream) {
    int openflag = (stream->sd.data == NULL);
    int error = 0;
    error = error | check_error_noquit((stream == NULL)
        , "stream_close : stream == NULL\n");
    error = error | check_error_noquit(openflag
        , "stream_close : Le flux n'est pas ouvert\n");
    if (error) {
        return -2;
    }
	return stream->op._close(&stream->sd);
}

int stream_read(stream_t* stream, void* buffer, size_t size) {
    int openflag = (stream->sd.data == NULL);
    int error = 0;
    error = error | check_error_noquit((stream == NULL)
        , "stream_read : stream == NULL\n");
    error = error | check_error_noquit(openflag
        , "stream_read : Le flux n'est pas ouvert\n");
    error = error | check_error_noquit((buffer == NULL)
        , "stream_read : buffer == NULL\n");
    error = error | check_error_noquit((size <= 0)
        , "stream_read : size <= 0\n");
    if (error) {
        return -2;
    }
	return stream->op._read(&stream->sd, buffer, size);
}

int stream_write(stream_t* stream, void* buffer, size_t size) {
    int openflag = (stream->sd.data == NULL);
    int error = 0;
    error = error | check_error_noquit((stream == NULL)
        , "stream_write : stream == NULL\n");
    error = error | check_error_noquit(openflag
        , "stream_write : Le flux n'est pas ouvert\n");
    error = error | check_error_noquit((buffer == NULL)
        , "stream_write : buffer == NULL\n");
    error = error | check_error_noquit((size <= 0)
        , "stream_write : size <= 0\n");
    if (error) {
        return -2;
    }
	return stream->op._write(&stream->sd, buffer, size);
}

int stream_unlink(stream_t* stream, const char* name) {
    int error = 0;
    error = error | check_error_noquit((stream == NULL)
        , "stream_unlink : stream == NULL\n");
    error = error | check_error_noquit((name == NULL)
        , "stream_unlink : name == NULL\n");
    if (error) {
        return -2;
    }
    manager_removestream(stream);
	return stream->op._unlink(name);
}
