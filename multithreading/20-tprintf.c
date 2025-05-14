#include "multithreading.h"
pthread_mutex_t *tprintf_mutex;

void init_mutex(void) __attribute__((constructor (101)));
void destroy_mutex(void) __attribute__((destructor (101)));

/**
 * init_mutex - constructor to initialize the mutex before main()
 */
void init_mutex(void)
{
	pthread_mutex_init(tprintf_mutex, NULL);
}

/**
 * destroy_mutex - destructor to destroy the mutex at program close
 */
void destroy_mutex(void)
{
	pthread_mutex_destroy(tprintf_mutex);
}

/**
 * tprintf - uses the printf family to print out a given formatted string
 * @format: string with specifiers
 *
 * Return: 0 if successful
 */
int tprintf(char const *format, ...)
{
	va_list args;

	pthread_mutex_lock(tprintf_mutex);
	printf("[%ld] ", pthread_self());

	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	pthread_mutex_unlock(tprintf_mutex);
	return (0);
}
