include ../../Makefile.inc

LIBNAME = libmq.so

all: $(LIBNAME)

se_mq.o : se_mq.c
$(LIBNAME) : se_mq.o
	$(CC) $(CFLAGS) -shared $(LDLIBS) -o $(LIBNAME) se_mq.o
	
install :
	mkdir -p ../../bin/plugin/stream/
	cp $(LIBNAME) ../../bin/plugin/stream/$(LIBNAME)
    
uninstall :
	rm -f ../../bin/plugin/stream/$(LIBNAME)
	rmdir --ignore-fail-on-non-empty ../../bin/plugin/stream/ ../../bin/
    
clean :
	rm -f *.o $(LIBNAME)
