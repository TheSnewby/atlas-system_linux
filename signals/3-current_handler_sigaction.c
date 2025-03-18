#include "signals.h"

/**
 * current_handler_sigaction - retrieves the current handler of the SIGINT
 *
 * Return: void pointer to current handler or NULL
 */
void (*current_handler_sigaction(void))(int)
{
	struct sigaction current;

	if (sigaction(SIGINT, NULL, &current) == -1)
		return (NULL);

	return (current);
}
