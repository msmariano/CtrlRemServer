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
#include "/home/msmariano/OpenWrt-SDK/package/CtrlRemServer/include/evento.h"

#define DEFAULT_COMM_DEVICE "/dev/ttyATH0"
#define ACTION_ACTGATE  "Act"
#define ACTION_TEST  "Test"
#define ACTION_CFG  "Cfg"
#define DEFAULT_BAUDRATE B9600
#define PORT_SERVER "8888"
#define _DEBUG
#define _SERIALON

#ifndef _CTRL_REM_SERVER_H
#define _CTRL_REM_SERVER_H
extern char *optarg;
extern int optind, opterr, optopt;
struct termios comm_config_orig; 
int CommPort; 
#endif

int InitLinhaSerie(char *LinhaSerie, struct termios *p_oldtio);
void writeToSerialLn(char * mens);
void * leSerial(void * arg);
void PerfomanceActionBySerial(char * act,int newsockfd,int test);

