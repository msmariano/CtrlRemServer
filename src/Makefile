# build CtrlRemServer executable when user executes "make"
CtrlRemServer: CtrlRemServer.o
	$(CC) $(LDFLAGS) CtrlRemServer.o -o CtrlRemServer -lpthread
CtrlRemServer.o: CtrlRemServer.c
	$(CC)  $(CFLAGS) -c CtrlRemServer.c -lpthread

# remove object files and executable when user executes "make clean"
clean:
	rm *.o CtrlRemServer
