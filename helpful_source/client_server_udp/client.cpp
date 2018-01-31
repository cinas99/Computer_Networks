#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFSIZE 100
#define PORT 8081

int main ()
{
	char buf[BUFSIZE];
	struct sockaddr_in sockServer, sockClient;
	socklen_t sockSize = sizeof(struct sockaddr);
	
	memset(&sockClient, 0, sizeof(struct sockaddr));
	sockClient.sin_family = AF_INET;
	sockClient.sin_addr.s_addr = INADDR_ANY; // jakikolwiek adres
	sockClient.sin_port = 0; // -> jakikolwiek port wybrany przez system

	memset(&sockServer, 0, sizeof(struct sockaddr));
	sockServer.sin_family = AF_INET;
	sockServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockServer.sin_port = htons(PORT);
	
	int nSocket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (nSocket < 0) {
		perror ("Nie można utworzyć gniazdka");
		exit (EXIT_FAILURE);
	}

	if (bind(nSocket, (struct sockaddr*)&sockClient, sizeof(struct sockaddr)) < 0) {
		perror ("Błąd funkcji bind");
		exit (EXIT_FAILURE);
	}

	if (inet_aton("127.0.0.1", &sockServer.sin_addr) == 0)
	{
		fprintf(stderr, "Błąd funkcji inet_aton");
		exit(1);
	}
	
	int n = sprintf(buf, "%s\n", "Hello UDP world!");
	int s = sendto(nSocket, buf, BUFSIZE, 0, (struct sockaddr*)&sockServer, sockSize);
	
	close (nSocket);

	exit (EXIT_SUCCESS);
}