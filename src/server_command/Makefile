include ../../Makefile.inc

LIBNAME = pid gid uid time add set exi con

all:
	-for f in $(LIBNAME); do \
		$(CC) $(CFLAGS) -shared -fPIC $(LDLIBS) -o lib$$f.so $$f.c; \
	done;
	
install :
	mkdir -p ../../bin/plugin/servercmd
	-for f in $(LIBNAME); do \
		cp lib$$f.so ../../bin/plugin/servercmd/; \
	done;
    
uninstall :
	rm -rf ../../bin/plugin/servercmd
	rmdir --ignore-fail-on-non-empty ../../bin/plugin/ ../../bin/
	
clean :
	rm -f *.o *.so
