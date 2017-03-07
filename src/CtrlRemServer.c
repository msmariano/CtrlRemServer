/*
 * CtrlRemServer.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: Marcelo dos Santos Mariano
 *  07/03/2017 - Inserido função de retorno do Arduino. Inserindo controle versao inicial V_1.0.0
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>	/* Configuração da linha série */
#include <unistd.h>
#include <signal.h>	/* Instalação de rotina para Ctrl-C */
#include <getopt.h>	/* Tratamento da linha de comando */
#include <fcntl.h>
#include <sys/types.h>

#include <errno.h>
#include <curses.h>	/* Gestão do teclado */
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

#define DEFAULT_COMM_DEVICE "/dev/ttyATH0"
#define DEFAULT_BAUDRATE B9600

#define _DEBUG

bool bAct;



/*
<?xml version="1.0" encoding="UTF-8"?>
 - <note>
       <to>Tove</to>
       <from>Jani</from>
       <heading>Reminder</heading>
       <body>Don't forget me this weekend!</body>
   </note>

<?xml version="1.0" encoding="UTF-8"?>
<objeto>
    <porteiro>
		<tipo>comando</tipo>
		<acao>0</acao>
		<imei>????????????????</imei>
	</porteiro>
</objeto>  

*/






typedef struct stPessoa
{
 char sNome[50];
 char sIMEI[100];
 int iPermissao;

}st_Pessoa;

/* Variáveis para o processamento das opções de linha de comando */
extern char *optarg;
extern int optind, opterr, optopt;
struct termios comm_config_orig; /* configuracao original */

/* Variáveis globais que armazenam vários valores de configuração */
/* Neste ponto são inicializadas com os valores "default" */
char CommDevice[32] = DEFAULT_COMM_DEVICE; /* Porta série */
int CommPort; /* file descriptor da porta série. */

/* comunicação serial */
int InitLinhaSerie(char *LinhaSerie, struct termios *p_oldtio) {
	int fd; /* Descriptor de ficheiro para a porta série */
	struct termios newtio;

	fd = open(LinhaSerie, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror("erro ao abrir porta serial");
		return -1;
	}

	/* Depois da porta aberta, guardar valores da configuração */
	/* anterior e criar a nova configuracao */
	tcgetattr(fd, p_oldtio); /* guarda valores antigos */
	bzero(&newtio, sizeof(newtio)); /* limpa nova configuracao */

	/*
	 Configuracao das c_flags

	 B19200 : Baudrate
	 CS8 : 8 data bits
	 CLOCAL : ligacao local, ignorar linhas de controle do modem
	 CREAD : autorizar recepcao de caracteres
	 sem CRTSCTS : sem controlo por hardware
	 */
	newtio.c_cflag = DEFAULT_BAUDRATE;
	newtio.c_cflag |= (CS8 | CLOCAL | CREAD);

	/*
	 Configuracao das c_iflags:
	 IGNPAR : ignorar erros de paridade;
	 ICRNL : transforma "carriage return" em "newline" à entrada.
	 */
	newtio.c_iflag = IGNBRK | IGNPAR | ICRNL;

	/* Configuracao das c_oflags */
	newtio.c_oflag = ONLCR;

	/* Configuracao das c_lflags */
	newtio.c_lflag = 0;

	/* Configuracao do timeout entre caracteres recebidos */
	newtio.c_cc[VTIME] = 0; /* timeout em 1/10s */
	newtio.c_cc[VMIN] = 0; /* não tem numero limite de caracteres */

	/* Limpar a linha série antes de activar a nova configuracao */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	return fd;
}

void fLogarErro(char * sErro)
{

}

char ** split(char * sMens, char * sSeparador)
{
}

bool bGravarPessoa(st_Pessoa stPessoa)
{
  return false;
}

bool bParseCfg(char * sMens)
{
  return false;
}

/*Thread para tratamento da entrada serial*/
void * leSerial(void * arg)
{
	int * iSocket = (int*)arg;
#ifdef _DEBUG
	printf("id do socket%d...\r\n",*iSocket);
#endif	
	
	int i = 0;
	int rc =0;
	int indice = 0;
	char c = '\0';
  	char bufferSerial[1024] = "";
	while(true)
	{		
#ifdef _DEBUG
		sleep(1);
		printf("Executando thread[%d]...\r\n",i++);
#endif
		rc = read(CommPort,&c, 1);
		if(rc == 1 &&c == '\n')
		{			
		  if(bAct)
		    write(*iSocket, bufferSerial, sizeof(bufferSerial));
		  indice = 0;
		  bzero(bufferSerial,1024);
		}
		else if(rc == 1)
		{
			bufferSerial[indice] = c;
			indice++;
			if(indice == 1024)
			{
				indice = 0;
				bzero(bufferSerial,1024);
			}
		}
	}
	
}


int main(int argc, char *argv[]) {

#ifdef _DEBUG
	printf("Iniciando...\r\n");
#endif

    
	char sMens[255];
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	st_Pessoa pMarcelo;
   bAct = false;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		fLogarErro("ERROR opening socket\n");

	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi("81");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		fLogarErro("ERROR on binding\n");
	}
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

#ifdef _SERIALON
	CommPort = InitLinhaSerie(CommDevice, &comm_config_orig);
#endif

	/*Inicializa thread para leitura da entrada serial*/
	pthread_t hTh;
	newsockfd = -1234;
	int iDTh = pthread_create(&hTh,NULL,leSerial,&newsockfd);	
	
#ifdef _DEBUG
	printf("Thread leitura serial iniciada...\r\n");
#endif

   
	while (true) {
		bAct = false;
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		memset(sMens,0,255);
		if (newsockfd < 0) {
			fLogarErro("ERROR on accept\n");
		}
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
			fLogarErro("ERROR reading from socket\n");
		}

		if (strstr(buffer, "act") != NULL) {

			write(CommPort, "Act", 3);
			write(CommPort, "\n", 1);
			sleep(1);
			strcpy(sMens,"Bemvindo!\n");

		} 
		else if (strstr(buffer, "teste") != NULL)
		{
			
		}
		else if (strstr(buffer, "cfg") != NULL) 
		{
		}	 
		else 
		{
		  strcpy(sMens,"Void\n");
		}
		if (send(newsockfd, sMens,sizeof(sMens), 0) == -1){
			fLogarErro("Erro enviando!\n");
		}

		close(newsockfd);

	}

	pthread_exit(NULL);
	return 0;

}
