#include "hnm.h"

/**
 * is_little_endian - Determine system endianness - apparently this works lol
 *
 * Return: 1 if little endian, 0 if big endian
 */
int is_little_endian(void)
{
	uint16_t num = 1;
		return (*(char *)&num == 1);
}

/**
 * swap_uint64 - Swap bytes for 64-bit integer if needed
 * @x: Value to potentially swap
 * @is_big_endian: Flag indicating if file uses big endian
 *
 * Return: Potentially byte-swapped value
 */
uint64_t swap_uint64(uint64_t x, int is_big_endian)
{
	if (is_little_endian() != !is_big_endian)
		return (bswap_64(x));
	return (x);
}

/**
 * swap_uint32 - Swap bytes for 32-bit integer if needed
 * @x: Value to potentially swap
 * @is_big_endian: Flag indicating if file uses big endian
 *
 * Return: Potentially byte-swapped value
 */
uint32_t swap_uint32(uint32_t x, int is_big_endian)
{
	if (is_little_endian() != !is_big_endian)
		return (bswap_32(x));
	return (x);
}


/**
 * swap_uint16 - Swap bytes for 32-bit integer if needed
 * @x: Value to potentially swap
 * @is_big_endian: Flag indicating if file uses big endian
 *
 * Return: Potentially byte-swapped value
 */
uint16_t swap_uint16(uint16_t x, int is_big_endian)
{
	if (is_little_endian() != !is_big_endian)
		return (bswap_16(x));
	return (x);
}
