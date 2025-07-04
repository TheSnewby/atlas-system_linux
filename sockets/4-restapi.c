#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 1024

/**
 * parseMessage - parses the received message, ASSUMES WELL-FORMED MESSAGE
 * @msg: received message
 * @size: size of message
 */
void parseMessage(char msg[], size_t size)
{
	size_t i = 0, sscanfRtn;
	char parse[10][BUFFER_SIZE], *token = NULL;
	char method[10], path[20], version[20];

	if (size < BUFFER_SIZE - 1)
		msg[size] = '\0';

	token = strtok(msg, "\n");
	while(token && i < 2)
	{
		strcpy(parse[i++], token);
		token = strtok(NULL, "\n");
	}

	sscanfRtn = sscanf(parse[0], "%s %s %s", method, path, version);
	if (sscanfRtn != 3)
	{
		perror("Error in sscanf: ");
		exit (EXIT_SUCCESS);
	}
	printf("Method: %s\nPath: %s\nVersion: %s\n", method, path, version);
	fflush(stdout);
}

/**
 * serverRecvAndSend - waits for a message and then prints the message
 * @clientfd: fd of the client
 */
void serverRecvAndSend(int clientfd)
{
	int flags = 0, bytesRecv = 0; /* for recv */
	char recvBuf[BUFFER_SIZE]; /* message from recv */
	size_t recvBufLen = BUFFER_SIZE; /* length of recv message */
	char success[] = "HTTP/1.1 200 OK\r\n\r\n";

	memset(recvBuf, 0, BUFFER_SIZE);

	bytesRecv = recv(clientfd, (void *)recvBuf, recvBufLen, flags);
	if (bytesRecv == 1024)
		printf("larger buffer required\n");
	else if (bytesRecv < 0)
	{
		perror("Error in recv");
		exit(EXIT_SUCCESS);
	}
	fflush(stdout);
	printf("Raw request: \"");
	printf("%.*s\"\n", bytesRecv, recvBuf);
	parseMessage(recvBuf, bytesRecv);
	send(clientfd, success, 18, 0);
}

/**
 * main - a program that opens an IPv4/TCP socket,
 * and listens to traffic on port 8080
 *
 * Return: 0
 */
int main(void)
{ /* fd is also the sockid, port assigned */
	int fd, clientfd = -2, port = 8080;
	int type = SOCK_STREAM; /* full-duplex byte stream */
	int domain = AF_INET, protocol = 0; /* IPv4 */ /* default protocol */
	struct sockaddr_in addrport, clientaddrport;
	socklen_t size = sizeof(struct sockaddr_in);
	int backlog = 10; /* queue for accept */

	addrport.sin_family = AF_INET; /* address family */
	addrport.sin_port = htons(port); /* converts host ord to network byte ord */
	addrport.sin_addr.s_addr = INADDR_ANY; /* sets ip addr to bind anything */

	fd = socket(domain, type, protocol); /* creates endpoint for communication */
	if (fd < 0)
	{
		perror("socket failed");
		exit(EXIT_SUCCESS);
	}
	/* binds a name to a socket in kernel */
	if (bind(fd, (struct sockaddr *)&addrport, size) < 0)
	{
		perror("bind failed");
		exit(EXIT_SUCCESS);
	}
	if (listen(fd, backlog) < 0) /* instructs kernel socket to listen */
	{
		perror("listen failed");
		exit(EXIT_SUCCESS);
	}
	printf("Server listening on port %d\n", port);
	while (1)
	{
		size = sizeof(struct sockaddr_in);
		clientfd = accept(fd, (struct sockaddr *)&clientaddrport, &size);
		while (clientfd == -2)
			;
		if (clientfd == -1) {
			perror("accept failed");
			continue;  // or handle appropriately
		}
		fflush(stdout);
		printf("Client connected: %s\n", inet_ntoa(clientaddrport.sin_addr));
		serverRecvAndSend(clientfd);
		close(clientfd);
		clientfd = -2;
	}
	return (0);
}
