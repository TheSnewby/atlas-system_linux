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
	int pidfd;
	pid_t pid = atoi(argv[1]);

	if (argc != 2)
	{
		printf("Usage: %s %d\n", argv[0], pid);
		return (EXIT_FAILURE);
	}

	pidfd = syscall(SYS_pidfd_open, pid, 0);

	if (syscall(SYS_pidfd_send_signal, pidfd, SIGINT, NULL, 0))
		{
			close(pidfd);
			return (EXIT_FAILURE);
		}
	close(pidfd);
	return (EXIT_SUCCESS);
}
