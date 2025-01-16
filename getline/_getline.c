#include "_getline.h"

/**
 * line_end_check - checks for newline \n
 * @input: unprocessed input
 * @size: size of input
 *
 * Return: 0 if not found, index of \n or EOF if found
 */
int line_end_check(char *input, int size)
{
	int i = 0;

	while (i < size)
	{
		if (input[i] == '\n')
			return (i);
		i++;
	}
	if (size < READ_SIZE)
		return (i);
	return (0);
}

/**
 * line_end - checks for end of line \n or EOF,
 * removes chars from line_end from input
 * @input: processed input
 * @input_size: size of input
 *
 * Return: returns a string up to \n, or string up to size if EOF
 */
char *line_end(char **input, int *input_size)
{
	int i = 0, line_length;
	char *line_end = NULL, *new_input = NULL;

	if (*input == NULL || *input_size <= 0)
		return (NULL);

	while (i < *input_size && (*input)[i] != '\n')  /* find \n or EOF */
		i++;

	line_length = i;  /* length of line_end */

	line_end = (char *)malloc((line_length + 1) * sizeof(char));
	if (line_end == NULL)
	{
		fprintf(stderr, "Malloc failed for line_end\n");
		return (NULL);
	}
	strncpy(line_end, *input, line_length);
	line_end[line_length] = '\0';
	/* remove the line_end from the original input string */
	int remaining_size = *input_size - line_length - 1;

	if (remaining_size > 0)
	{
		new_input = (char *)malloc(remaining_size * sizeof(char));
		if (new_input == NULL)
		{
			fprintf(stderr, "Malloc failed for new_input\n");
			free(line_end);
			return (NULL);
		}
		if (i < *input_size && (*input)[i] == '\n')
			memcpy(new_input, *input + line_length + 1, remaining_size);
		else
			memcpy(new_input, *input + line_length, remaining_size);
	}
	free(*input);
	*input = new_input;
	*input_size = remaining_size;
	return (line_end);
}

/**
 * reset_getline - resets and frees static and dynamic variables
 * @fd: file descriptor
 * @input: dynamic string containing possible read input
 * @input_size: tracks number of chars in input
 *
 * Return: void
 */
void reset_getline(const int *fd, char **input, int *input_size)
{
	*input_size = 0;
	if (input && *input)
	{
		free(*input);
		*input = NULL;
	}
	if (fd)
		close(*fd);
}

/**
 * _getline - reads an entire line from a file descriptor
 * @fd: file descriptor
 *
 * Return: null-terminated text read from fd, or null if none
 */
char *_getline(const int fd)
{
	char buf[READ_SIZE] = {0};  /* buffer of all input sans previous lines*/
	static char *input;  /* static buffer for processing lines */
	char *line = NULL;  /* output line */
	int rd_rtn = 0;  /* read return value */
	static int input_size;  /* tracks number of chars in input */

	if (!input)
	{
		input = (char *)malloc(READ_SIZE * sizeof(char));
		memset(input, 0, READ_SIZE);
		input_size = 0;
	}

	if (fd < 0)
	{
		reset_getline(&fd, &input, &input_size);
		return (NULL);
	}

	while (!line_end_check(input, input_size))  /* loop until \n or EOF found */
	{
		rd_rtn = read(fd, buf, READ_SIZE);
		if (rd_rtn > 0)
		{
			input = (char *)realloc(input, (input_size + rd_rtn + 1) * sizeof(char));
			memcpy(input + input_size, buf, rd_rtn);
			input_size += rd_rtn;
			input[input_size] = '\0';
		}
		else if (rd_rtn <= 0)  /* break if no more data to read */
			break;
	}
	if (input_size > 0)
		line = line_end(&input, &input_size);
	return (line);
}
