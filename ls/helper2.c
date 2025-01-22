#include "_ls.h"

/**
 * _strcpy - copies a string to another string
 * @dest: destination string
 * @src: source string
 *
 * Return: pointer to dest
 */
char *_strcpy(char *dest, char *src)
{
	int i;

	if (!dest || !src)
		return (NULL);

	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];
	dest[i] = '\0';

	return (dest);
}

/**
 * remove_dot_slash - copies a string to another string while removing first
 * two characters
 * @dest: destination string
 * @src: source string
 *
 * Return: pointer to dest
 */
char *remove_dot_slash(char *dest, char *src)
{
	int i;

	if (!dest || !src)
		return (NULL);

	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i + 2];
	dest[i] = '\0';

	return (dest);
}
