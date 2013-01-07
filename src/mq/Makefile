include ../../Makefile.inc

LIBNAME = libmq.so

all: $(LIBNAME)

se_mq.o : se_mq.c
$(LIBNAME) : se_mq.o
	$(CC) $(CFLAGS) -shared $(LDLIBS) -o $(LIBNAME) se_mq.o
	
install :
	mkdir -p ../../bin/plugin/
	cp $(LIBNAME) ../../bin/plugin/$(LIBNAME)
    
uninstall :
	rm -f ../../bin/plugin/$(LIBNAME)
	rmdir --ignore-fail-on-non-empty ../../bin/plugin/ ../../bin/
    
clean :
	rm -f *.o $(LIBNAME)
