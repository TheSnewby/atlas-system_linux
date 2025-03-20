#include <sys/types.h>
#include <signal.h>

/**
 * main - tests if a process exists, given its PID
 * @pid: process id
 *
 * Return: 1 if pid exists, 0 otherwise
 */
int pid_exist(pid_t pid)
{
	return ((kill(pid, 0) == 0) ? 1 : 0);
}
