#include "_getline.h"

/**
 * _strlen - returns the lengths of a string
 * @str: string
 *
 * Return: length of string
 */
int _strlen(char *str)
{
	int i;
	for (i = 0; str[i] != '\0'; i++)
		continue;
	return (i);
}

/**
 * line_end_check - checks for end of line \n or EOF
 * @buf: passed buffer
 * @size: size of buffer
 *
 * 	Return: returns a string up to \n, or string up to size if EOF
 */
char *line_end_fix(char buf[], int size)
{
	int i = 0;
	char *new_buf = NULL;

	while (i < size)
	{
		if(buf[i] == '\n')
		{
			new_buf = (char *)malloc((i + 1) * sizeof(char));
			if (new_buf == NULL)
			{
				fprintf(stderr, "Malloc Fail in line_end_check");
				return (NULL);
			}
			new_buf = strncpy(new_buf, buf, i);
			new_buf[i] = '\0';
			return (new_buf);
		}
		i++;
	}
	new_buf = (char *)malloc((i + 1) * sizeof(char));
	if (new_buf == NULL)
	{
		fprintf(stderr, "Malloc Fail in line_end_check");
		return (NULL);
	}
	new_buf = strncpy(new_buf, buf, size);
	new_buf[i] = '\0';
	return (new_buf);
}

/**
 * line_end_check - checks for newline \n
 * @buf: passed buffer
 * @size: size of buffer
 *
 * 	Return: 0 if not found, 1 if found
 */
int line_end_check(char buf[], int size)
{
	int i = 0;

	while (i < size)
	{
		if(buf[i] == '\n')
			return (1);
		i++;
	}
	if (size < READ_SIZE)
		return (1);
	return (0);
}

/**
 * malloc_buffer - mallocs a buffer into a string
 * @buf: buffer
 * @size: size of buffer
 *
 * Return - malloc'd string or NULL if failure
 */
char *malloc_buffer(char buf[], int size)
{
	char *new_buf = (char *)malloc((size) * sizeof(char));
	if (new_buf == NULL)
	{
		fprintf(stderr, "Malloc Fail in line_end_check");
		return (NULL);
	}
	new_buf = strncpy(new_buf, buf, size);
	new_buf[size] = '\0';
	return (new_buf);
}

/**
 * _getline - reads an entire line from a file descriptor
 * @fd: file descriptor
 *
 * Return: null-terminated text read from fd, or null if none
 */
char *_getline(const int fd)
{
	char buf[READ_SIZE] ={0};  /* buffer */
	char *full_read = NULL;  /* the full text up to end of line or file */
	int rd_rtn = 0;  /* return value of the read() */
	char *full_read_tracker;  /* ptr to current final location */

	if (fd < 0)
		return (NULL);

	rd_rtn = read(fd, buf, READ_SIZE);
	if (rd_rtn <= 0)  /* if error or EOF return NULL */
	{
		close(fd);
		return (NULL);
	}
	while (rd_rtn > 0)
	{
		if (!full_read)  /* initial empty */
		{
			if(line_end_check(buf, rd_rtn))
			{
				full_read = line_end_fix(buf, rd_rtn);
				return (full_read);
			}
			full_read = malloc_buffer(buf, rd_rtn);
		}
		else
		{
			memset(buf, 0, READ_SIZE);
			rd_rtn = read(fd, buf, READ_SIZE);
			if (rd_rtn == 0)
				close(fd);
			else
			{
				full_read = realloc(full_read, (_strlen(full_read) + rd_rtn + 1)
				* sizeof(char));
				if (full_read == NULL)
				{
					close(fd);
					return (NULL);
				}
				full_read_tracker = full_read + _strlen(full_read);
				strcpy(full_read_tracker, buf);
			}
		}
	}
	full_read = realloc(full_read, (_strlen(full_read) + 1) * sizeof(char));
	if (full_read == NULL)
	{
		close(fd);
		return (NULL);
	}
	full_read[_strlen(full_read)] = '\0';
	return (full_read);
}
