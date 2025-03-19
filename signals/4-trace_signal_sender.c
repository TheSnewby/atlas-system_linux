#include "signals.h"

/**
 * sigquit_handler - handler for SIGQUITs
 * @newact: sigaction
 *
 * Return: void
 */
void sigquit_handler(int sig, siginfo_t *info, void *ucontext)
{
	printf("SIGQUIT sent by [%d]\n", info->pid_t);
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
