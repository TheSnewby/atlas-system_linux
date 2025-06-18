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
 * serverRecvAndSend - waits for a message and then prints the message
 * @clientfd: fd of the client
 */
void serverRecvAndSend(int clientfd)
{
	int flags = 0, bytesRecv = 0; /* for recv */
	char recvBuf[BUFFER_SIZE]; /* message from recv */
	size_t recvBufLen = BUFFER_SIZE; /* length of recv message */

	memset(recvBuf, 0, BUFFER_SIZE);

	bytesRecv = recv(clientfd, (void *)recvBuf, recvBufLen, flags);
	if (bytesRecv == 1024)
		printf("larger buffer required\n");
	else if (bytesRecv < 0)
	{
		perror("Error in recv");
		exit(EXIT_SUCCESS);
	}
	printf("Message received: \"");
	printf("%.*s\"\n", bytesRecv, recvBuf);
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

	clientfd = accept(fd, (struct sockaddr *)&clientaddrport, &size);
	while (clientfd == -2)
		;

	printf("Server listening on port %d\n", port);
	printf("Client connected: %s\n", inet_ntoa(clientaddrport.sin_addr));
	serverRecvAndSend(clientfd);
	/* Print the full received HTTP request
	Print the break-down of the first line of the received HTTP request (See example)
	Send back a response to the connected client (HTTP 200 OK)
	Close the connection with the client
	Wait for the next connection */
	close(clientfd);
	return (0);
}
