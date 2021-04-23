#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

void *potok(void *arg)
{
	int Clientsock = *((int*)arg);
	pthread_detach(pthread_self());
	char message[BUFFER_SIZE];
	int msgLenght;
	bzero(message, BUFFER_SIZE);
	while ((msgLenght = recv(Clientsock,message,BUFFER_SIZE,0)))
				{
					printf("Get message: %s\n",message);
				}
	close (Clientsock);
	pthread_exit(0);
	return NULL;
}


int main()
{
	pthread_t tid;
	int Mainsock, Clientsock, length;
	struct sockaddr_in serverAddr;
	if ((Mainsock = socket(AF_INET, SOCK_STREAM,0))<0)
	{
			perror("Error in socket");
			exit(1);
	}
	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddr.sin_port=htons(0);
	if (bind(Mainsock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
	{
		perror("Error bind");
		exit(1);
	}
	length=sizeof(serverAddr);
	if(getsockname(Mainsock, (struct sockaddr*)&serverAddr, &length))
	{
		perror("Error getsockname");
		exit(1);
	}
	printf("Port = %d\n",htons(serverAddr.sin_port));
	listen(Mainsock,5);
	for(;;)
	{
		if ((Clientsock = accept(Mainsock,0,0)) < 0)
		{
			perror("Accept error");
			exit(1);
		}
		pthread_create(&tid,0,potok,(void*)&Clientsock);
	}
	return 0;
}
