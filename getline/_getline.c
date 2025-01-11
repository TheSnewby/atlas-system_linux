#include "_getline.h"

/**
 * _getline - reads an entire line from a file descriptor
 * @fd: file descriptor
 *
 * Return: text read, or null if none
 */
char *_getline(const int fd)
{
	int i;
	char buf[READ_SIZE];
	char *full_read = NULL;
	int fd_rtn = 0, rd_rtn = 0;

	// check fd is good
	// loop reading until \n or eof
	// 	check if read
	//   add to full_read until
	// return full_read or null
}
