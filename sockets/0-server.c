#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

/**
 * main - a program that opens an IPv4/TCP socket,
 * and listens to traffic on port 12345
 *
 * Return: 0
 */
int main(void)
{
	int fd, status, port = 12345; /* fd is also the sockid, port assigned */
	int type = SOCK_STREAM; /* full-duplex byte stream */
	int domain = AF_INET; /* IPv4 */
	int protocol = 0; /* default */
	struct sockaddr_in addrport;
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
	status = bind(fd, (struct sockaddr *)&addrport, size);
	if (status < 0)
	{
		perror("bind failed");
		exit(EXIT_SUCCESS);
	}

	status = listen(fd, backlog); /* instructs kernel socket to listen */
	if (status < 0)
	{
		perror("listen failed");
		exit(EXIT_SUCCESS);
	}

	while (1)
		;

	return (0);
}
