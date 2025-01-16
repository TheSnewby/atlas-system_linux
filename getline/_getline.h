#ifndef getline_h
#define getline_h
#define READ_SIZE 1024
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *_getline(const int);
int line_end_check(char buf[], int);
int alloc_check(char *string);

/**
 * struct fd_state - states of field descriptors
 * @fd: fd
 * @input: unprocessed buffer
 * @input_size: current size of input
 * @next: next node of fd_state
 *
 * This stuct tracks fds with associated variables.
 */
typedef struct fd_state
{
	int fd;
	char *input;
	int input_size;
	struct fd_state *next;
} fd_state_n;

#endif
