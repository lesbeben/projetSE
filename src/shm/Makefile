include ../../Makefile.inc

LIBNAME = libshm.so

all: $(LIBNAME)

se_shm.o : se_shm.c
$(LIBNAME) : se_shm.o
	$(CC) $(CFLAGS) -shared $(LDLIBS) -o $(LIBNAME) se_shm.o
	
install :
	mkdir -p ../../bin/plugin/stream/
	cp $(LIBNAME) ../../bin/plugin/stream/$(LIBNAME)
    
uninstall :
	rm -f ../../bin/plugin/stream/$(LIBNAME)
	rmdir --ignore-fail-on-non-empty ../../bin/plugin/stream/ ../../bin/
	
clean :
	rm -f *.o $(LIBNAME)
