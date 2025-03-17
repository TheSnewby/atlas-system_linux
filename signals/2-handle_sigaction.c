#include "signals.h"

/**
 * sigint_handler - handler for SIGINTs
 * @sig: signal number
 *
 * Return: void
 */
void sigint_handler(int sig)
{
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
	struct sigaction newact;

	newact.sa_handler = sigint_handler;

	if (sigaction(SIGINT, &newact, NULL) == -1)
		return (-1);

	return (0);
}
