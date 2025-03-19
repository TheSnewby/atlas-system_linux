#include "signals.h"

/**
 * main - sends the signal SIGINT to a process, given its PID
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: %s %s\n", argv[0], argv[1]);
		return (EXIT_FAILURE);
	}

	if (syscall(SYS_pidfd_send_signal,atoi(argv[1]), SIGINT, NULL, 0))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
