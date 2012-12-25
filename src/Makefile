CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -D_XOPEN_SOURCE=600
LDLIBS = -lrt -lpthread

all: server client

se_fifo.o : se_fifo.c se_fifo.h stream.h error.h
se_shm.o : se_shm.c se_shm.h stream.h error.h
se_mq.o : se_mq.c se_mq.h stream.h error.h
stream.o : stream.c stream.h error.h
stream_manager.o : stream_manager.c stream_manager.h
error.o : error.c error.h
server.o: server.c 
server : server.o se_mq.o se_fifo.o se_shm.o stream.o error.o stream_manager.o
client : client.o se_mq.o se_fifo.o se_shm.o stream.o error.o stream_manager.o

clean :
	rm -f *.o server client
