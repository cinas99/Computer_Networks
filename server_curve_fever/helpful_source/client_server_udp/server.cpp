#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define BUF_SIZE 100
#define PORT 8081

int main(int argc, char* argv[])
{
	char buf[BUF_SIZE];
	struct sockaddr_in sockServer, sockClient;
	socklen_t sockClientLen = sizeof(sockClient);

	/* address structure */
	memset(&sockServer, 0, sizeof(struct sockaddr));
	sockServer.sin_family = AF_INET;
	sockServer.sin_addr.s_addr = htonl(INADDR_ANY);
	sockServer.sin_port = htons(PORT);

	/* create a socket */
	int nSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (nSocket < 0) {
		perror ("Nie można utworzyć gniazdka");
		exit (EXIT_FAILURE);
	}
	
	/* bind a name to a socket */
	if (bind(nSocket, (struct sockaddr*)&sockServer, sizeof(struct sockaddr)) < 0) {
		perror ("Błąd funkcji bind");
		exit (EXIT_FAILURE);
	}
	
	
	int rcvLen = recvfrom(nSocket, buf, BUF_SIZE, 0, (struct sockaddr*)&sockClient, &sockClientLen);
	printf("Odebrano pakiet od %s:%d\n", inet_ntoa(sockClient.sin_addr), ntohs(sockClient.sin_port));
    printf("Długość wiadomości: %d, Treść: %s\n" , rcvLen, buf);

	close(nSocket);
	return(0);
}
