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

    if (src[0] == '.' && src[1] == '/') { /* if string starts with ./ */
        for (i = 0; src[i + 2] != '\0'; i++) { /* skip em */
            dest[i] = src[i + 2];
        }
        dest[i] = '\0';
    }

	else { /* otherwise, just copy string */
        _strcpy(dest, src);
    }

    return (dest);
}

/**
 * _memcpy - copies memory area
 * @dest: copy to
 * @src: copy from
 * @n: number of bytes copied
 *
 * Return: pointer to memory area
*/
char *_memcpy(char *dest, char *src, int n)
{
	int i;

	for (i = 0; i < n; i++)
		dest[i] = src[i];

	return (dest);
}

/**
 * _strcmp - compares two strings
 * @s1: 1st string
 * @s2: 2nd string
 *
 * Return: 0 if equal, < 0 if s1 < s2, > 0 otherwise
*/
int _strcmp(char *s1, char *s2)
{
	int diff = 0;
	int i;
	int s1end = 1;
	int s2end = 1;

	for (i = 0; s1end != 0 && s2end != 0; i++)
	{
		if (s1[i] == '\0')
			s1end = 0;
		if (s2[i] == '\0')
			s2end = 0;
		if (s1[i] != s2[i])
		{
			diff += s1[i] - s2[i];
			break;
		}
	}

	return (diff);
}