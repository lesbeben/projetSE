DIRS = src/mq src/shm src/fifo src/

all :
	-for d in $(DIRS); do (cd $$d; $(MAKE) all ); done

clean:
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

install:
	-for d in $(DIRS); do (cd $$d; $(MAKE) install ); done

uninstall:
	-for d in $(DIRS); do (cd $$d; $(MAKE) uninstall ); done
