include ../../Makefile.inc

LIBNAME = libshm.so

all: $(LIBNAME)

se_shm.o : se_shm.c
$(LIBNAME) : se_shm.o
	$(CC) $(CFLAGS) -shared $(LDLIBS) -o $(LIBNAME) se_shm.o
	
install :
	mkdir -p ../../bin/plugin/
	cp $(LIBNAME) ../../bin/plugin/$(LIBNAME)
    
uninstall :
	rm -f ../../bin/plugin/$(LIBNAME)
	rmdir --ignore-fail-on-non-empty ../../bin/plugin/ ../../bin/
	
clean :
	rm -f *.o $(LIBNAME)
