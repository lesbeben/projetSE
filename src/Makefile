CC = gcc
CFLAGS = -Wall -pedantic -std=c99
LDLIBS = -lrt -lpthread

all: server

se_fifo.o : se_fifo.c se_fifo.h stream.h error.h
se_shm.o : se_shm.c se_shm.h stream.h error.h
se_mq.o : se_mq.c se_mq.h stream.h error.h
stream.o : stream.c stream.h error.h
error.o : error.c error.h
server.o: server.c 
server : server.o se_mq.o se_fifo.o se_shm.o stream.o error.o

clean :
	rm -f *.o server
