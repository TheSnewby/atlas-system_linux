#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

/**
 * main - a program that connects to a listening server
 * @argc: number of arguments
 * @argv: array of string arguments
 *
 * Return: 0 on success, EXIT_FAILURE otherwise
 */
int main(int argc, char **argv)
{
	int fd, port;
	int type = SOCK_STREAM; /* full-duplex byte stream */
	int domain = AF_INET, protocol = 0; /* IPv4 */ /* default protocol */
	struct sockaddr_in foreignAddr;

	if (argc != 3)
	{
		printf("Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	port = atoi(argv[2]);
	foreignAddr.sin_family = AF_INET;
	foreignAddr.sin_port = htons(port);

	if (strcmp(argv[1], "localhost") == 0)
		foreignAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	else
		foreignAddr.sin_addr.s_addr = inet_addr(argv[1]);

	fd = socket(domain, type, protocol);
	if (fd < 0)
	{
		perror("Failed to Create a Socket");
		return (EXIT_FAILURE);
	}

	if (connect(fd, (struct sockaddr *)&foreignAddr, sizeof(foreignAddr)))
	{
		perror("Failed to Connect");
		return (EXIT_FAILURE);
	}
	return (0);
}
