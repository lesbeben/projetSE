CC = gcc
CFLAGS = -Wall -pedantic -std=c99
LDLIBS = -lrt -lpthread

all: main

se_fifo.o : se_fifo.c se_fifo.h stream.h
se_shm.o : se_shm.c se_shm.h stream.h
se_mq.o : se_mq.c se_mq.h stream.h
stream.o : stream.c stream.h
main.o : main.c const.h
main: main.o se_mq.o se_fifo.o se_shm.o stream.o


clean :
	rm -f *.o 
