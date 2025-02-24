#include "main.h"

/**
 * fifthSet - prints Size of this header & Size of program headers
 * @buf: buffer containing the first 64 bytes
 */
void fifthSet(unsigned char buf[])
{
	uint16_t entryOneSix;
	int isSixFour = (buf[4] == 1) ? 0 : 1;

	/* Size of this header */
	printf("  Size of this header:               ");
	if (isSixFour)
	{
		buildOneSix(&entryOneSix, buf, 0x34);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d (bytes)\n", (unsigned char)entryOneSix);
	}
	else
	{
		buildOneSix(&entryOneSix, buf, 0x28);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d (bytes)\n", (unsigned char)entryOneSix);
	}

	/* Size of program headers */
	printf("  Size of program headers:           ");
	if (isSixFour)
	{
		buildOneSix(&entryOneSix, buf, 0x36);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d (bytes)\n", (unsigned char)entryOneSix);
	}
	else
	{
		buildOneSix(&entryOneSix, buf, 0x2A);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d (bytes)\n", (unsigned char)entryOneSix);
	}
}

/**
 * sixthSet - prints Number of progam headers & Sie of Section headers
 * @buf: buffer containing the first 64 bytes
 */
void sixthSet(unsigned char buf[])
{
	uint16_t entryOneSix;
	int isSixFour = (buf[4] == 1) ? 0 : 1;

	/* Number of program headers */
	printf("  Number of program headers:         ");
	if (isSixFour)
	{
		buildOneSix(&entryOneSix, buf, 0x38);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d\n", (unsigned char)entryOneSix);
	}
	else
		{
			buildOneSix(&entryOneSix, buf, 0x2C);
			if (buf[5] == 2)
				entryOneSix = bswap_16(entryOneSix);
			printf("%d\n", (unsigned char)entryOneSix);
		}

	/* Size of section headers */
	printf("  Size of section headers:           ");
	if (isSixFour)
	{
		buildOneSix(&entryOneSix, buf, 0x3A);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d (bytes)\n", (unsigned char)entryOneSix);
	}
	else
	{
		buildOneSix(&entryOneSix, buf, 0x2E);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d (bytes)\n", (unsigned char)entryOneSix);
	}
}

/**
 * seventhSet - prints number of section headers & Section header str table ind
 * @buf: buffer containing the first 64 byttes
 */
void seventhSet(unsigned char buf[])
{
	uint16_t entryOneSix;
	int isSixFour = (buf[4] == 1) ? 0 : 1;

	/* Number of section headers */
	printf("  Number of section headers:         ");
	if (isSixFour)
	{
		buildOneSix(&entryOneSix, buf, 0x3C);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d\n", (unsigned char)entryOneSix);
	}
	else
	{
		buildOneSix(&entryOneSix, buf, 0x30);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d\n", (unsigned char)entryOneSix);
	}

	/* Section header string table index */
	printf("  Section header string table index: ");
	if (isSixFour)
	{
		buildOneSix(&entryOneSix, buf, 0x3E);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d\n", (unsigned char)entryOneSix);
	}
	else
	{
		buildOneSix(&entryOneSix, buf, 0x32);
		if (buf[5] == 2)
			entryOneSix = bswap_16(entryOneSix);
		printf("%d\n", (unsigned char)entryOneSix);
	}
}
