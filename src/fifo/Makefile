include ../../Makefile.inc

LIBNAME = libfifo.so

all: $(LIBNAME)

se_fifo.o : se_fifo.c
$(LIBNAME) : se_fifo.o
	$(CC) $(CFLAGS) -shared $(LDLIBS) -o $(LIBNAME) se_fifo.o
	
install :
	mkdir -p ../../bin/plugin/
	cp $(LIBNAME) ../../bin/plugin/$(LIBNAME)
    
uninstall :
	rm -f ../../bin/plugin/$(LIBNAME)
	rmdir --ignore-fail-on-non-empty ../../bin/plugin/ ../../bin/
	
clean :
	rm -f *.o $(LIBNAME)
