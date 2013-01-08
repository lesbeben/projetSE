include ../../Makefile.inc

LIBNAME = libfifo.so

all: $(LIBNAME)

se_fifo.o : se_fifo.c
$(LIBNAME) : se_fifo.o
	$(CC) $(CFLAGS) -shared $(LDLIBS) -o $(LIBNAME) se_fifo.o
	
install :
	mkdir -p ../../bin/plugin/stream/
	cp $(LIBNAME) ../../bin/plugin/stream/$(LIBNAME)
    
uninstall :
	rm -f ../../bin/plugin/stream/$(LIBNAME)
	rmdir --ignore-fail-on-non-empty ../../bin/plugin/stream/ ../../bin/
	
clean :
	rm -f *.o $(LIBNAME)
