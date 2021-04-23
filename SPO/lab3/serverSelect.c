#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

int main()
{
	int Mainsock, Clientsock, length;
	struct sockaddr_in serverAddr;
	fd_set rset, allset;
	int maxi, maxfd, i, sockfd;
	int client[FD_SETSIZE], nready;
	char message[BUFFER_SIZE];
	int msgLenght;
	if ((Mainsock = socket(AF_INET, SOCK_STREAM,0))<0)
	{
			perror("Error in socket");
			exit(1);
	}
	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddr.sin_port=0;
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
	
	maxfd = Mainsock;
	maxi = -1;
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;
		
	FD_ZERO(&rset);
	FD_SET(Mainsock, &allset);
	
	for(;;)
	{
		rset = allset;
		nready = select (maxfd + 1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(Mainsock,&rset))
		{
			Clientsock = accept(Mainsock,0,0);
			
			for (i = 0;i < FD_SETSIZE; i++)
			if (client[i] < 0)
				{
					client[i] = Clientsock;
					break;
				}
			if (i == FD_SETSIZE)
				perror("Server are busy, try later");
				
			FD_SET(Clientsock, &allset);
			if (Clientsock > maxfd)
				maxfd = Clientsock;
			if (i > maxi)
				maxi = i;
				
			if (--nready <= 0)
				continue;
		}
		for (i = 0; i <= maxi; i++)
		{
			if ((sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd,&rset))
			{
				bzero(message, BUFFER_SIZE);
				if ((msgLenght = recv(sockfd, message, BUFFER_SIZE, 0)) == 0)
				{
					close(sockfd);
					FD_CLR(sockfd,&allset);
					client[i] = -1;
				}else printf("Message recived: %s\n", message);
				if (--nready <= 0)
					break;
			}
		}
	}
	return 0;
}
