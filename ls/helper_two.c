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
 * @msg_num: error type (1 = non-fatal, 2 = fatal)
 * @program_name: name of program
 * @file_path: file path where error occurred
 * @errnum: errno of error
 * @str_one: string that needs freeing
 * @str_two: string that needs freeing
 *
 */
void print_error(int msg_num, char* program_name, char *file_path, int errnum,
char *str_one, char *str_two)
{
    /* figure out which message should be printed */
    if (errnum == 2) /* file not found */
        fprintf(stderr, "%s: cannot access '%s': ", program_name, file_path);
    else if (errnum == 13) /* permission denied */
        fprintf(stderr, "%s: cannot open directory '%s': ", program_name, file_path);

    perror(NULL); /* system error message */

    /* clean-up for non-fatal */
    if (str_one)
        free(str_one);
    if (str_two)
        free(str_two);

	/* fatal errors (exit)*/
    if (msg_num == 2)
        exit(errnum);
}
