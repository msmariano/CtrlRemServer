/*
 * CtrlRemServer.cpp
 *
 *  Criação: 21/08/2015
 *      Autor: Marcelo dos Santos Mariano
 *  07/03/2017 - Inserido função de retorno do Arduino. 
 					- Inserindo controle versão inicial V_1.0.0.
 	 08/03/2017 - Organizando Projeto
 *  
 */

#include "/home/msmariano/OpenWrt-SDK/package/CtrlRemServer/include/CtrlRemServer.h"


int main(int argc, char *argv[]) 
{
#ifdef _DEBUG
	printf("Iniciando CtrlRemServer[Arduino] V_1.0.2...\r\n");
	printf("Porta do Servidor [%s]...\r\n",PORT_SERVER);
#endif

	char sMens[255];
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	{
#ifdef _DEBUG
		printf("Erro abrindo socket...\r\n");
#endif
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(PORT_SERVER);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	{
#ifdef _DEBUG
		printf("Erro na função bind...\r\n");
#endif	
		exit(1);
	}
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

#ifdef _SERIALON
#ifdef _DEBUG
   printf("Interface Serial [%s]...\r\n",DEFAULT_COMM_DEVICE);
#endif
	CommPort = InitLinhaSerie(DEFAULT_COMM_DEVICE, &comm_config_orig);
#endif

#ifdef _DEBUG
   printf("Servidor iniciado id[%d]...\r\n",sockfd);
#endif

   
	while (true) 
	{
	
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		memset(sMens,0,255);
		if (newsockfd < 0) 
		{
#ifdef _DEBUG
		   printf("Erro na função accept...\r\n");
#endif			
			continue;
		}
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n <= 0) 
		{
#ifdef _DEBUG
		   printf("Erro na função read...\r\n");
#endif
			continue;			
		}
#ifdef _DEBUG
		   printf("Mensagem recebida : %s\r\n",buffer);
#endif

		if (strstr(buffer, "act") != NULL) 
		  PerfomanceActionBySerial(ACTION_ACTGATE,newsockfd,0);
		else if (strstr(buffer, "test") != NULL)
	     PerfomanceActionBySerial(ACTION_TEST,newsockfd,0);
		else if (strstr(buffer, "cfg") != NULL) 
		  PerfomanceActionBySerial(ACTION_CFG,newsockfd,0);
		else if ( strstr(buffer, ACTION_TESTNET) != NULL)
		{
			PerfomanceActionBySerial(ACTION_TESTNET,newsockfd,1);
		}
		else 
		{
		  if (send(newsockfd, "Nenhuma comando recebido...",sizeof("Nenhuma comando recebido..."), 0) == -1)
	     { 
#ifdef _DEBUG
		printf("Erro enviando socket[2]...\r\n");
#endif
		  }
		}
		
		close(newsockfd);

	}
	pthread_exit(NULL);
	return 0;

}
