include ../Makefile.inc

SERVER   = server
CLIENT   = client
OBJFILES = stream.o stream_manager.o stream_set.o error.o project.o
SERVEROBJ = server.o server_request.o server_signal.o child_list.o child_process.o
CLIENTOBJ = client.o client_request.o client_signal.o

all: $(SERVER) $(CLIENT)

%.o: %.c
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ -c $<

$(SERVER) : $(SERVEROBJ) $(OBJFILES)
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVEROBJ) $(OBJFILES) $(LDLIBS)

$(CLIENT) : $(CLIENTOBJ) $(OBJFILES)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENTOBJ) $(OBJFILES) $(LDLIBS)
		
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
