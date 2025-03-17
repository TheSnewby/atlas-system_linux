#include "signals.h"

/**
 * sigint_handler - handler specifically for SIGINT
 * @signal: signal number
 */
void sigint_handler(int signal)
{
	if (signal == SIGINT)
		printf("Gotcha! [%d]\n", SIGINT);
}

/**
 * handle_signal - a function that sets a handler for the signal SIGINT
 *
 * Return: 0 on sucesss, -1 on error
 */
int handle_signal(void)
{
	int signal_rtn;
	fflush(NULL);

	signal_rtn = signal(SIGINT, sigint_handler);
	if (signal_rtn == SIG_ERR)
	{
		printf("Failure to set handler for SIGINT\n");
		return (-1);
	}
	return (0);
}
