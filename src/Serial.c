#include "/home/msmariano/OpenWrt-SDK/package/CtrlRemServer/include/CtrlRemServer.h"




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

void writeToSerialLn(char * mens)
{
	write(CommPort,mens,strlen(mens));
	write(CommPort,"\n",1);
	sleep(1);
}



/*Thread para tratamento da entrada serial*/
void * leSerial(void * arg)
{
	HANDLE * ev  = (HANDLE*)arg;	
		
#ifdef _DEBUG
	printf("id do socket[%d]...\r\n",ev->idSocket);
#endif	
	
	int i = 0;
	int rc =0;
	int indice = 0;
	char c = '\0';
  	char bufferSerial[1024] = "";
	while(ev->bGo)
	{		
#ifdef _DEBUG
		sleep(1);
		printf("Executando thread[%d]...\r\n",i++);
#endif
		rc = read(CommPort,&c, 1);
		if(rc == 1 &&c == '\n')
		{			
		  if (send(ev->idSocket, bufferSerial,sizeof(bufferSerial), 0) == -1)
		  {
			  logar("Erro enviando!\n");
		  }			  
		  indice = 0;
		  bzero(bufferSerial,1024);
		  SetEvent(ev);
		  break;
		}
		else if(rc == 1)
		{
			bufferSerial[indice] = c;
			indice++;
			if(indice == 1024)
			{
				indice = 0;
				bzero(bufferSerial,1024);
				 SetEvent(ev);
		       break;
			}
		}
	}
	
}
//void PerfomanceActionBySerial(char * act,int newsockfd,int test=0);
void PerfomanceActionBySerial(char * act,int newsockfd,int test)
{
  HANDLE * ev = CreateEvent(NULL);
  if(!test)
    writeToSerialLn(act);
  else 
  {
  	 if (send(newsockfd, "Testando...\n",sizeof("Testando..."), 0) == -1)
	 {
			  logar("Erro enviando!\n");
	 }
	 return;
  }
  pthread_t hTh;
  ev->idSocket = newsockfd;
  int iDTh = pthread_create(&hTh,NULL,leSerial,ev);
  if(!WaitForEvent(ev,5))
  {
#ifdef _DEBUG
	printf("Failed...\r\n");
#endif
    tcflush(CommPort, TCIFLUSH);
	 ev->bGo = false;
	 if (send(newsockfd, "Falhou performance serial...\n",sizeof("Falhou performance serial...\n"), 0) == -1)
	 {
			  logar("Falhou performance serial...\n");
	 }
	 
  }
  else 
  {
#ifdef _DEBUG
	printf("Sucesso...\r\n");
#endif
  	
  }
  free(ev);
}