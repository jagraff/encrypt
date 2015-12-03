#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define MAXSIZE 1024   /* Size of receive buffer */

void DieWithError(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}


int main(int argc, char **argv)
{
	int sock;                        /* Socket descriptor */
	struct sockaddr_in servAddr; /* Echo server address */
	unsigned short servPort;     /* Echo server port */
	long key;
	int i = 0;
	int size;

	long encrypted_buffer[MAXSIZE];
	char buffer[MAXSIZE];


	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s <Host> <Port Number> <key>\n", argv[0]);
		exit(1);
	}
	struct hostent *he;
	char *serverName = argv[1];
	// get server ip from server name
	if ((he = gethostbyname(serverName)) == NULL) {
		exit(1);
	}
	char *serverIP = inet_ntoa(*(struct in_addr *)he->h_addr);


	servPort = atoi(argv[2]);

	key = atol(argv[3]);

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket() failed");

	memset(&servAddr, 0, sizeof(servAddr));     /* Zero out structure */
	servAddr.sin_family      = AF_INET;             /* Internet address family */
	servAddr.sin_addr.s_addr = inet_addr(serverIP);   /* Server IP address */
	servAddr.sin_port        = htons(servPort); /* Server port */

	/* Establish the connection to the server */
	
	if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		DieWithError("connect() failed");

	fgets(buffer, MAXSIZE, stdin);

	memset(encrypted_buffer, '\0', MAXSIZE);

	size = strlen(buffer);

	while (i < size) {
		encrypted_buffer[i] = ((long)buffer[i]) * key;
		i++;
	}


	send(sock, encrypted_buffer, size * sizeof(long), 0);

	size = recv(sock, encrypted_buffer, MAXSIZE, 0);

	i = 0;

	while (i <= size) {
		encrypted_buffer[i++] /= key;
	}

	send(sock, encrypted_buffer, size, 0);

	close(sock);
	return(0);
}
