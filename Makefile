DIRS = src/ \
       src/stream \
       src/client_command src/server_command

all :
	-for d in $(DIRS); do (cd $$d; $(MAKE) all ); done

clean:
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

install:
	-for d in $(DIRS); do (cd $$d; $(MAKE) install ); done

uninstall:
	-for d in $(DIRS); do (cd $$d; $(MAKE) uninstall ); done
