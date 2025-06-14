#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

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
	int backlog = 10;

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

	printf("Server listening on port %s\n", inet_ntoa(clientaddrport.sin_addr));
	printf("Client connected: %d\n", ntohs(clientaddrport.sin_port));
	close(clientfd);

	return (0);
}
