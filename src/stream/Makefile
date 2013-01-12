include ../../Makefile.inc

LIBNAME = fifo mq shm

all:
	-for f in $(LIBNAME); do \
		$(CC) $(CFLAGS) -shared $(LDLIBS) -o lib$$f.so $$f.c; \
	done;
	
install :
	mkdir -p ../../bin/plugin/stream/
	-for f in $(LIBNAME); do \
		cp lib$$f.so ../../bin/plugin/stream/; \
	done;
    
uninstall :
	rm -rf ../../bin/plugin/stream
	rmdir --ignore-fail-on-non-empty ../../bin/plugin/ ../../bin/
	
clean :
	rm -f *.o *.so
