#include "_ls.h"

/**
 * adjust_long_time - handles ls long printing of the last modified time
 * @str: string of time
 *
 * Return: modified string
 *
 * This can be further modified to replace hours:minutes with year if
 * it's not the current year
 */
char *adjust_long_time(char *str)
{
	int i = 0;

	if (str == NULL)
		return (NULL);

	while (i < 4)
	{
		i++;
		str++;
	}

	i = 0;
	while (str[i] != '\0')
		i++;

	str[i - 8] = '\0';
	return (str);
}

/**
 * print_error - prints errors
 * @msg_num: int denoting which message to print
 * @program_name: name of program
 * @file_path: file path
 * @errnum: errno of error
 * @str_one: possible malloc'd str that needs freeing
 * @str_two: possible malloc'd str that needs freeing
 *
 * Return: 0 if it shouldn't quit, or passed errno to exit ?
 */
void print_error(int msg_num, char* program_name, char *file_path, int errnum,
char *str_one, char *str_two)
{
	/* initial thoughts */
	if (msg_num == 1)  /* cannot access file */
	{
		fprintf(stderr, "%s: cannot access %s: ", program_name, file_path);
		perror(NULL);
	}

	if (msg_num == 2)  /* cannot access file and exit */
	{
		fprintf(stderr, "%s: cannot access %s: ", program_name, file_path);
		perror(NULL);
		exit(errnum);
	}

	if (str_one)
	{
		free(str_one);
		str_one = NULL;
	}
	if (str_two)
	{
		free(str_two);
		str_two = NULL;
	}
}
