#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>


int main(argc,argv)
int argc; char* argv[];
{
	
	int sock;
	struct sockaddr_in serverAddr;
	struct hostent *hp, *gethostbyname();
	
	if (argc < 4)
	{
		printf("No all arguments");
		exit(1);
	}
	if ((sock=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("Error socket");
		exit(1);
	}
	bzero ((char*) &serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	bcopy(hp->h_addr,&serverAddr.sin_addr, hp->h_length);
	serverAddr.sin_port = htons(atoi(argv[2]));
	if (connect(sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0)
	{
		perror("Error connet");
		exit(1);
	}
	int i = 0;
	while(i < 20)
	{
		if (send(sock,argv[3],strlen(argv[3]),0)<0)
		{
			perror("Error send");
			exit(1);
		}
		sleep(atoi(argv[3]));
		i++;
	}
	printf("Send ok");
	close(sock);
	exit(0);
}
