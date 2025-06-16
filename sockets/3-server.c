#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

/**
 * serverRecv - waits for a message and then prints the message
 * @clientfd: fd of the client
 */
void serverRecv(int clientfd)
{
	int flags = 0, bytesRecv = 0; /* for recv */
	char recvBuf[1024]; /* message from recv */
	size_t recvBufLen = 1024; /* length of recv message */

	memset(recvBuf, 0, 1024);

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
 * and listens to traffic on port 12345
 *
 * Return: 0
 */
int main(void)
{ /* fd is also the sockid, port assigned */
	int fd, clientfd = -2, port = 12345;
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
	serverRecv(clientfd);
	close(clientfd);
	return (0);
}
