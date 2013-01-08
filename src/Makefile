include ../Makefile.inc

SERVER   = server
CLIENT   = client
OBJFILES = stream.o stream_manager.o stream_set.o error.o

all: $(SERVER) $(CLIENT)

%.o: %.c
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ -c $<

$(SERVER) : server.o server_request.o $(OBJFILES)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(SERVER) server.o server_request.o $(OBJFILES)

$(CLIENT) : client.o client_request.o $(OBJFILES)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(CLIENT) client.o client_request.o $(OBJFILES)
		
install :
	mkdir -p ../bin/
	cp $(CLIENT) ../bin/$(CLIENT)
	cp $(SERVER) ../bin/$(SERVER)
    
uninstall :
	rm -f ../bin/$(CLIENT)
	rm -f ../bin/$(SERVER)
	rmdir --ignore-fail-on-non-empty ../bin/

clean :
	rm -f *.o $(SERVER) $(CLIENT)
