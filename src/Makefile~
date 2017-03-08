# build CtrlRemServer executable when user executes "make"
CtrlRemServer: CtrlRemServer.o
	$(CC) $(LDFLAGS) CtrlRemServer.o -o CtrlRemServer Serial.o evento.o -lpthread
CtrlRemServer.o: CtrlRemServer.c
	$(CC)  $(CFLAGS) -c CtrlRemServer.c   Serial.c  evento.c -lpthread

# remove object files and executable when user executes "make clean"
clean:
	rm *.o CtrlRemServer 
