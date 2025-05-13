#include "syscalls.h"
/* printf(fmt, ...) -> fprintf(stderr, fmt, ...)*/

/**
 * execute_command - executes command and attaches tracer
 * Each time a syscall is intercepted,
 * you must print its number, followed by a new line
 * @argv: user-input command
 * @envp: environment
 *
 * Return: 0 if successful, otherwise a negative value
 */
int execute_command(char **argv, char **envp)
{
	pid_t pid;
	int status, ptrace_rtn, syscall, i = 0;
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
		if (ptrace_rtn == -1)
		{
			perror("PTRACE_TRACEME Failed");
			exit(EXIT_FAILURE);
		}

		raise(SIGSTOP);

		if (execve(argv[1], &argv[1], envp) == -1)
			perror("execve failed"), exit(EXIT_FAILURE);
	} /* parent process */
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("Error at waitpid");
			return (-1);
		}

		while (1)
		{
			ptrace_rtn = ptrace(PTRACE_SYSCALL, pid, 0, 0);
			if (ptrace_rtn == -1)
			{
				perror("PTRACE_SYSCALL Failed");
				exit(EXIT_FAILURE);
			}
			if (waitpid(pid, &status, 0) == -1)
			{
				perror("Error at waitpid");
				return (-1);
			}

			if (WIFEXITED(status))
				break;

			ptrace_rtn = ptrace(PTRACE_GETREGS, pid, 0, &regs);
			if (ptrace_rtn == -1)
			{
				perror("PTRACE_GETREGS Failed");
				exit(EXIT_SUCCESS);
			}
			syscall = regs.orig_rax;
			if ((i % 2 != 0) && (i != 0))
			{
				// fprintf(stderr, "%d: ", syscall); // DEBUGGING
				fprintf(stderr, "%s\n", syscalls_64_g[syscall].name);
			}
			i++;
		}
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
int main(int argc, char **argv, char **envp)
{
	if (argc < 2)
	{
		fprintf(stderr, "Needs at least 1 argument.\n");
		return (0);
	}

	execute_command(argv, envp);
	return (0);
}
