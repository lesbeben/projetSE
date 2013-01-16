include ../../Makefile.inc

LIBNAME = pid gid uid time add set exi

all:
	-for f in $(LIBNAME); do \
		$(CC) $(CFLAGS) -shared $(LDLIBS) -o lib$$f.so $$f.c; \
	done;
	
install :
	mkdir -p ../../bin/plugin/clientcmd
	-for f in $(LIBNAME); do \
		cp lib$$f.so ../../bin/plugin/clientcmd/; \
	done;
    
uninstall :
	rm -rf ../../bin/plugin/clientcmd
	rmdir --ignore-fail-on-non-empty ../../bin/plugin/ ../../bin/
	
clean :
	rm -f *.o *.so
