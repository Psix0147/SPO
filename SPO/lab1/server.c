#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>

#define BUFFER_SIZE 1024

void ZombHandler(int s)
{
	pid_t pid;
	int sig;
	while ((pid = waitpid(-1,&sig,WNOHANG)) > 0);
	return;
}

int main()
{
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
	signal(SIGCHLD,ZombHandler);
	for(;;)
	{
		char message[BUFFER_SIZE];
		int msgLenght;
		if ((Clientsock = accept(Mainsock,0,0)))
		{
			int f = fork();
			if (f < 0) perror("Error fork");
			if (f == 0)
			{
				close (Mainsock);
				bzero(message, BUFFER_SIZE);
				while ((msgLenght = recv(Clientsock,message,BUFFER_SIZE,0)))
				{
					printf("Get message: %s\n",message);
				}
				close (Clientsock);
				exit(1);
			}
			close (Clientsock);
		}
	}
}
