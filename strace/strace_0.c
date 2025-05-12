#include "syscalls.h"


/**
 * execute_command - executes command and attaches tracer
 * Each time a syscall is intercepted,
 * you must print its number, followed by a new line
 * @argv: user-input command
 * 
 * Return: 0 if successful, otherwise a negative value
 */
int execute_command(char **argv)
{
	pid_t pid;
	int status, ptrace_rtn, syscall;
	struct user_regs_struct regs;

	pid = fork();
	if (pid < 0)
	{
		perror("failed fork");
		return (0);
	}
	else if (pid == 0) /* child process */
	{
		ptrace_rtn = ptrace(PTRACE_TRACEME, 0, NULL, 0);
		execve(argv[1], argv + 1, NULL); // might have to pull in environ extern
		exit(EXIT_FAILURE);
	} /* parent process */
	else
	{
		while (1)
		{
			if (wait(&status) == -1)
			{
				perror("Error at waitpid");
				return (-1);
			}
			ptrace_rtn = ptrace(PTRACE_SYSCALL, pid, 0, 0);
			if (ptrace_rtn == -1)
			{
				perror("SYSCALL Failed");
				exit(EXIT_FAILURE);
			}

			if (WIFEXITED(status))
				break;

			ptrace_rtn = ptrace(PTRACE_GETREGS, pid, 0, &regs);
			syscall = regs.orig_rax;
			printf("%d\n", syscall);
		}

		// printf("%d\n", ptrace_rtn);
	}

	return (0);
}

/**
 * main - a program that executes and traces a given command
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: 0 if successful, -1 otherwise
 */
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Needs at least 1 argument.\n");
		return (0);
	}

	execute_command(argv);
	return (0);
}
