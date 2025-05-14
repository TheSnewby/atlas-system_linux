#include "multithreading.h"
/* also consider initializing like this instead of using constructor: */
/* phtread_mutex_t tprintf_mutex = PTHREAD_MUTEX_INITIALIZER */
#define NB_THREADS  10

/**
 * init_mutex - constructor to initialize the mutex before main()
 */
void init_mutex(void)
{
	if (pthread_mutex_init(&tprintf_mutex, NULL) == -1)
	{
		perror("mutex init failed");
		exit(1);
	}
}

/**
 * destroy_mutex - destructor to destroy the mutex at program close
 */
void destroy_mutex(void)
{
	if (pthread_mutex_destroy(&tprintf_mutex) == -1)
	{
		perror("mutex destroy failed");
		exit(1);
	}
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

	pthread_mutex_lock(&tprintf_mutex);
	printf("[%ld] ", pthread_self());

	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	pthread_mutex_unlock(&tprintf_mutex);
	return (0);
}

// #define NB_THREADS  10

// /**
//  * _thread_entry - Thread entry point. Prints out a string
//  *
//  * @arg: Unused
//  *
//  * Return: NULL
//  */
// void *_thread_entry(__attribute__((unused))void *arg)
// {
//     tprintf("Hello from thread\n");
//     return (NULL);
// }

// /**
//  * main - Entry point
//  *
//  * Return: EXIT_SUCCESS
//  */
// int main(void)
// {
//     pthread_t tid[NB_THREADS];
//     int i;

//     for (i = 0; i < NB_THREADS; i++)
//         pthread_create(&tid[i], NULL, &_thread_entry, NULL);

//     tprintf("Hello from main thread\n");

//     for (i = 0; i < NB_THREADS; i++)
//         pthread_join(tid[i], NULL);

//     return (EXIT_SUCCESS);
// }