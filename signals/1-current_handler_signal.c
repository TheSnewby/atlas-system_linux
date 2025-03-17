#include "signals.h"

/**
 * current_handler_signal - retrives the current handler of the signal SIGINT
 */
void (*current_handler_signal(void))(int)
{
	__sighandler_t sighandler = signal(SIGINT, SIG_IGN);

	if (sighandler == SIG_ERR)
		return (NULL);
	if (signal(SIGINT, sighandler) == SIG_ERR)  /* revert it back */
		return (NULL);
	return (sighandler);
}
