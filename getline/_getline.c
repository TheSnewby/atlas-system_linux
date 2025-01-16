#include "_getline.h"

/**
 * alloc_check - checks dynamic allocation didn't cause an issue
 * @string: dynamically allocated string
 *
 * Return: 1 if successful, 0 if successful
 */
int alloc_check(char *string)
{
	if (!string)  /* if NULL */
	{
		fprintf(stderr, "Memory allocation failed\n");
		return (1);
	}
	return (0);
}

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
	int i = 0, line_length, remaining_size;
	char *line_end = NULL, *new_input = NULL;

	if (*input == NULL || *input_size <= 0)
		return (NULL);

	while (i < *input_size && (*input)[i] != '\n')  /* find \n or EOF */
		i++;
	line_length = i;  /* length of line_end */
	line_end = (char *)malloc((line_length + 1) * sizeof(char));
	if (alloc_check(line_end))
		return (NULL);
	if (line_end == NULL)
	{
		fprintf(stderr, "Malloc failed for line_end\n");
		return (NULL);
	}
	strncpy(line_end, *input, line_length);
	line_end[line_length] = '\0';
	remaining_size = *input_size - line_length - 1;
	if (remaining_size > 0)
	{
		new_input = (char *)malloc(remaining_size * sizeof(char));
		if (alloc_check(new_input))
			return (NULL);
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
 * get_fd_state - gets or creates the fd_state for the current fd
 * @fd: fd
 * @head: current head of linked list
 *
 * Return: first fd_state
 */
fd_state_n *get_fd_state(int fd, fd_state_n **head)
{
	fd_state_n *current = *head;

	while (current)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}

	current = malloc(sizeof(fd_state_n));
	if (!current)
		return (NULL);

	current->fd = fd;
	current->input = NULL;
	current->input_size = 0;
	current->next = *head;
	*head = current;

	return (current);
}

/**
 * remove_fd_state - removes and frees fd_state from list
 * @head: head of liest
 * @fd: fd
 *
 * Return: void
 */
void remove_fd_state(fd_state_n **head, int fd)
{
	fd_state_n *current = *head, *prev = NULL;

	while (current)
	{
		if(current->fd == fd)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;

			free(current->input);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
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
	char *line = NULL;  /* output line */
	int rd_rtn = 0;  /* read return value */
	static fd_state_n *head;
	fd_state_n *fd_state;

	if (fd < 0)
	{
		remove_fd_state(&head, fd);
		if (fd != -1)
			return (NULL);
	}
	fd_state = get_fd_state(fd, &head);
	if (!fd_state)
		return (NULL);
	if (!fd_state->input)
	{
		fd_state->input = malloc(READ_SIZE);
		if (alloc_check(fd_state->input))
			return (NULL);
		fd_state->input_size = 0;
	}
	while (!line_end_check(fd_state->input, fd_state->input_size))
	{
		rd_rtn = read(fd, buf, READ_SIZE);
		if (rd_rtn > 0)
		{
			fd_state->input = realloc(fd_state->input, fd_state->input_size +
			rd_rtn + 1);
			if (alloc_check(fd_state->input))
				return NULL;

			memcpy(fd_state->input + fd_state->input_size, buf, rd_rtn);
			fd_state->input_size += rd_rtn;
			fd_state->input[fd_state->input_size] = '\0';
		}
		else if (rd_rtn <= 0)
			break;
	}
	if (fd_state->input_size > 0)
		line = line_end(&fd_state->input, &fd_state->input_size);

	return (line);
}
