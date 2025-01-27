#include "_ls.h"

/**
 * adjust_long_time - handles ls long printing of the last modified time
 * @str: string of time
 *
 * Return: modified string
 */
char *adjust_long_time(char *str)
{
	int i = 0;

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
