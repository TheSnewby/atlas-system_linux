#include "syscalls.h"


/**
 * execute_command - executes command and attaches tracer
 * @argv: user-input command
 * 
 * Return: 0 if successful, otherwise a negative value
 */
int execute_command(char **argv)
{
	pid_t pid, child_pid;
	int status, execveRtn, ptrace_rtn;

	pid = fork();
	if (pid < 0)
	{
		perror("failed fork");
		return (0);
	}
	else if (pid == 0) /* child process */
	{
		child_pid = getpid();
		ptrace_rtn = ptrace(PTRACE_ATTACH, child_pid, 0, 0);



		execveRtn = execve(argv[1], argv, NULL); // might have to pull in environ extern
		exit(EXIT_FAILURE);
	} /* parent process */
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("Error at waitpid");
			return (-2);
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
