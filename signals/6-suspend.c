#include "signals.h"

/**
 * sigint_handler - handler specifically for SIGINT
 * @signal: signal number
 */
void sigint_handler(int signal)
{
	printf("Caught %d\n", signal);
	if (signal == SIGINT)
		printf("Signal Received\n");
}

/**
 * handle_signal - a function that sets a handler for the signal SIGINT
 *
 * Return: 0 on sucesss, -1 on error
 */
int handle_signal(void)
{
	signal(SIGINT, sigint_handler);
	pause();  /* catches all signals? */

	return (EXIT_SUCCESS);
}
