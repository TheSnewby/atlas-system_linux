#include "multithreading.h"

/**
 * trpintf - uses the printf family to print out a given formatted string
 * @format: string with specifiers
 *
 * Return: 0 if successful
 */
int tprintf(char const *format, ...)
{
	va_list args;

	printf("[%ld] ", pthread_self());

	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	return (0);
}
