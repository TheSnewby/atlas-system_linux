#include "signals.h"

/**
 * sigint_handler - handler specifically for SIGINT
 * @signal: signal number
 */
void sigint_handler(int signal)
{
	printf("Caught %d\n", signal);
	if (signal == SIGINT)
		printf("Signal received\n");
}

/**
 * main - a function that waits indefinitely to handle the signal SIGINT
 *
 * Return: 0 on sucesss, -1 on error
 */
int main(void)
{
	signal(SIGINT, sigint_handler);
	pause();  /* catches all signals? */

	return (EXIT_SUCCESS);
}
