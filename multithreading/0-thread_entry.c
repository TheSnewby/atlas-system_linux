#include "multithreading.h"

/**
 * thread_entry - entry point to a new thread
 * @arg: holds the address of a string that must be printed
 * and followed by a new line
 */
void *thread_entry(void *arg)
{
	printf("%s\n", (char *)arg);
	pthread_exit(arg);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS
 */
int main(void)
{
    pthread_t tid;

    pthread_create(&tid, NULL, &thread_entry, "Holberton School");

    sleep(1);
    printf("Created thread ID -> %lu\n", tid);

    thread_entry("C is fun");
    printf("Should not be printed\n");
    return (EXIT_SUCCESS);
}