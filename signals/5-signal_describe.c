#include "signals.h"

/**
 * main - start of program
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: 0 if successful, other if not
 */
int main(int argc, char **argv)
{
	int signum = atoi(argv[1]);

	if (argc != 2)
		printf("Usage: %s %d\n", argv[0], signum);

	printf("%d: %s\n", signum, strsignal(signum));

	return (0);
}
