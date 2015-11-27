#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXPENDING 5
#define MAXSIZE 4096

void die(char *error)
{
	perror(error);
	exit(1);
}

int main(int argc, char **argv)
{
	int servSock;
	int clientSock;
	unsigned short servPort;
	unsigned int clientLen;
	struct sockaddr_in servAddr; /* Local address */
	struct sockaddr_in clientAddr; /* Client address */
	int recvMsgSize;
	int key;

	int encrypted_buffer[MAXSIZE];


	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <server_port> "
				"<key>\n" , argv[0]);
		exit(0);
	}

	servPort = atoi(argv[1]);

	key = atoi(argv[2]);


	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		die("socket() failed");

	/* Construct local address structure */
	memset(&servAddr, 0, sizeof(servAddr));   /* Zero out structure */
	servAddr.sin_family = AF_INET;                /* Internet address family */
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	servAddr.sin_port = htons(servPort);      /* Local port */

	

	/* Bind to the local address */
	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		die("bind() failed");

	if (listen(servSock, MAXPENDING) < 0)
		die("listen() failed");
	

	clientLen = sizeof(clientAddr);
	if ((clientSock = accept(servSock, (struct sockaddr *) &clientAddr, 
					&clientLen)) < 0)
		die("accept() failed");

	for (;;)
	{
		int i;

		if ((recvMsgSize = recv(clientSock, encrypted_buffer, MAXSIZE*sizeof(int), 0)) < 0)
			die("recv() failed");

		for (i = 0; i < recvMsgSize; i++) {
			encrypted_buffer[i] *= key;
		}

		send(clientSock, encrypted_buffer, recvMsgSize*sizeof(int), 0);

		if ((recvMsgSize = recv(clientSock, encrypted_buffer, MAXSIZE*sizeof(int), 0)) < 0)
			die("recv() failed");

		for (i = 0; i < recvMsgSize; i++) {
			putchar(encrypted_buffer[i] / key);
		}

	}
	
	close(clientSock);
	return 0;
}
