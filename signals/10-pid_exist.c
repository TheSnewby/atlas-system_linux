#include <stdlib.h>
#include <signal.h>

/**
 * main - tests if a process exists, given its PID
 * @argc: number of arguments
 * @argv: array of strings
 *
 * Return: 1 if pid exists, 0 otherwise
 */
int main(int argc, char **argv)
{
	return (((argc == 2) && (kill(atoi(argv[1]), 0) == 0)) ? 1 : 0);
}
