#include "signals.h"

/**
 * sigquit_handler - handler for SIGQUITs
 * @sig: signal number
 * @info: info from siginfo_t
 * @ucontext: further context if needed
 *
 * Return: void
 */
void sigquit_handler(int sig, siginfo_t *info, void *ucontext)
{
	(void)ucontext;
	(void)sig;

	printf("SIGQUIT sent by [%d]\n", info->si_pid);
	fflush(stdout);
}

/**
 * trace_signal_sender - defiens a handler for the signal SIGQUIT
 *
 * Return: 0 on sucess, -1 on error
 */
int trace_signal_sender(void)
{
	struct sigaction newact;

	newact.sa_sigaction = sigquit_handler;
	newact.sa_flags = SA_SIGINFO;

	if (sigaction(SIGQUIT, &newact, NULL) == -1)
		return (-1);

	return (0);
}
