#include "_getline.h"

/**
 * line_end_check - checks for newline \n
 * @buf: passed buffer
 *
 * 	Return: NULL if not found, adjusted buffer if found
 */
char *line_end_check(char buf[], int size)
{
	int i = 0;
	char *new_buf = NULL;

	while (i < size)
	{
		if(buf[i] == '\n')
		{
			new_buf = strncpy(new_buf, buf, i);
			return (new_buf);
		}
		i++;
	}
	return (NULL);
}

/**
 * _getline - reads an entire line from a file descriptor
 * @fd: file descriptor
 *
 * Return: null-terminated text read from fd, or null if none
 */
char *_getline(const int fd)
{
	char buf[READ_SIZE];  /* buffer */
	char *full_read = NULL;  /* the full text */
	int rd_rtn = 0;  /* return value of the read() */
	int full_read_size;  /* the size of the full_read variable */
	char *full_read_tracker;  /* ptr to current final location */

	if (fd < 0)
		return (NULL);

	rd_rtn = read(fd, buf, READ_SIZE);
	if (rd_rtn < 0)
	{
		close(fd);
		return (NULL);
	}
	while (rd_rtn > 0)
	{
		if (!full_read)  /* initial empty */
		{
			full_read = (char *)malloc(rd_rtn * sizeof(char));
			if (full_read == NULL)
			{
				close(fd);
				return (NULL);
			}
			strcpy(full_read, buf);
		}
		else
		{
			full_read_size = strlen(full_read);
			rd_rtn = read(fd, buf, READ_SIZE);
			if (rd_rtn == 0)
				close(fd);
			else
			{
				full_read = realloc(full_read, (full_read_size + rd_rtn)
				* sizeof(char));
				if (full_read == NULL)
				{
					close(fd);
					return (NULL);
				}
				full_read_tracker = full_read + full_read_size;
				strcpy(full_read_tracker, buf);
			}
		}
	}
	full_read_size = strlen(full_read);
	full_read = realloc(full_read, (full_read_size + 1) * sizeof(char));
	if (full_read == NULL)
	{
		close(fd);
		return (NULL);
	}
	full_read[full_read_size] = '\0';
	return (full_read);
}
