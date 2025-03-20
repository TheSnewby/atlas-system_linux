#include <stdlib.h>
#include <signal.h>
//only two headers

/**
 * main - tests if a process exists, given its PID
 * @argc: number of arguments
 * @argv: array of strings
 *
 * Return: 1 if pid exists, 0 otherwise
 */
int main(int argc, char **argv)
{
	return (kill(atoi(argv[1]), 0) == 0) ? (1) : (0);
}
