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
 * line_end_fix - checks for end of line \n or EOF
 * @buf: passed buffer
 * @size: size of buffer
 *
 * Return: returns a string up to \n, or string up to size if EOF
 */
char *line_end_fix(char buf[], int size)
{
	int i = 0;
	char *new_buf = NULL;

	while (i < size)
	{
		if (buf[i] == '\n')
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
 * Return: 0 if not found, 1 if found
 */
int line_end_check(char buf[], int size)
{
	int i = 0;

	while (i < size)
	{
		if (buf[i] == '\n')
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
 * Return: malloc'd string or NULL if failure
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
	static char buf[READ_SIZE] = {0};  /* buffer */
	char *full_read = NULL, *partial_read = NULL;  /* text up to \n/EOF, buf */
	int rd_rtn = 0;  /* return value of the read() */

	// printf("DBUG: fd is %d\n", fd);
	if (fd < 0)
		return (NULL);

	while ((rd_rtn = read(fd, buf, READ_SIZE)) > 0)
	{
		if (!full_read)  /* initial empty */
		{
			if (line_end_check(buf, rd_rtn))
			{
				full_read = line_end_fix(buf, rd_rtn);
				return (full_read);
			}
			else
				full_read = malloc_buffer(buf, rd_rtn);
		}
		else
		{
			if (line_end_check(buf, rd_rtn))
				partial_read = line_end_fix(buf, rd_rtn);  /* check rtn? */
			else
				partial_read = malloc_buffer(buf, rd_rtn);
			full_read = realloc(full_read, (_strlen(full_read) +
			_strlen(partial_read) + 1) * sizeof(char));
			if (full_read == NULL)
			{
				close(fd);
				free(partial_read);
				return (NULL);
			}
			full_read[_strlen(full_read)] = '\0';
			strcpy(full_read + _strlen(full_read), partial_read);
			free(partial_read);
		}
	}
	if (rd_rtn <= 0)  /* if error or EOF return NULL */
	{
		printf("DEBUG: rd_rtn <= 0 at line 149\n");
		close(fd);
		return (NULL);
	}
	return (full_read);
}
