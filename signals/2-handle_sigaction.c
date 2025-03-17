#include "signals.h"

/**
 * sigint_handler - handler for SIGINTs
 * @sig: signal number
 *
 * Return: void
 */
void sigint_handler(int sig)
{
	if (sig == SIGINT)
		printf("Gotcha! [%d]\n", sig);
	fflush(NULL);
}

/**
 * handle_sigaction - function that sets a handler for SIGINT
 *
 * Return: 0 on success, -1 on error
 */
int handle_sigaction(void)
{
	//int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
	struct sigaction newact;

	newact.sa_handler = sigint_handler;
	/* sigemptyset(&newact.sa_mask); */
	/* newact.sa_flags = 0; */

	if (sigaction(SIGINT, &newact, NULL) == -1)
		return (-1);

	return (0);
}
