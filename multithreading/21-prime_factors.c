#include "list.h"
#include <math.h>
#include <malloc.h>
#include <stdlib.h>

/**
 * sqrtul - sqrt for unsigned longs
 * @num: unsigned long
 * source: https://en.wikipedia.org/wiki/Integer_square_root
 *
 * Return: unsigned long integer sqrt
 */
unsigned long sqrtul(unsigned long num)
{
	unsigned long L = 0, M, R = num + 1;

	if (num == 0)
		return (0);

	while (R - 1 != L)
	{
		M = (L + R) / 2;
		if (M * M <= num)
			L = M;
		else
			R = M;
	}

	return (L);
}

/**
 * factor_list_add - adds a prime factor to the list
 * @list: list
 * @p: prime factor to be added
 * note: NOT MEMORY SAFE, but required with lack of control of list.c
 *
 * Return: void
 */
void factor_list_add(list_t *list, unsigned long p)
{
	unsigned long *pfac = malloc(sizeof(unsigned long));

	*pfac = p;
	list_add(list, pfac);
}

/**
 * prime_factors - factorizes a number in a list of prime factors
 * @s: string representation of the number to factorize
 *
 * Return: list
 */
list_t *prime_factors(char const *s)
{
	list_t *list = (list_t *)malloc(sizeof(list_t));
	char *endptr;
	unsigned long p = 2, num = strtoul(s, &endptr, 10), max_p = sqrtul(num);

	list = list_init(list);
	if (num < 2)
		return (NULL);
	while (num % p == 0)
	{
		factor_list_add(list, p);
		num /= p;
	}
	for (p = 3; p <= num / p && p <= max_p; p += 2)
	{
		while (num % p == 0)
		{
			factor_list_add(list, p);
			num /= p;
		}
	}
	// p = 3;
	// while (num % p == 0)
	// {
	// 	factor_list_add(list, p);
	// 	num /= p;
	// }
	// /* Sieve of Eratosthenes where possible prime = 6k +- 1, n > 0 */
	// for (k = 1; 6 * k + 1 <= max_p; k += 1)
	// {
	// 	p = (6 * k - 1);
	// 	while (num % p == 0)
	// 	{
	// 		factor_list_add(list, p);
	// 		num /= p;
	// 		max_p = sqrtul(num);
	// 	}
	// 	p = (6 * k + 1);
	// 	while (num % p == 0)
	// 	{
	// 		factor_list_add(list, p);
	// 		num /= p;
	// 		max_p = sqrtul(num);
	// 	}
	// }
	if (num > 1)
		factor_list_add(list, num);

	return (list);
}
