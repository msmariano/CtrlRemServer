#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>	
#include <getopt.h>	
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <curses.h>	
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>

typedef struct
{
	sem_t sema;
	int val;
	char * str;
	int idSocket;
	bool bGo;
}HANDLE;

HANDLE * CreateEvent(char * str);
int WaitForEvent(HANDLE * id,int time);
void SetEvent(HANDLE *  id);

