#include "main.h"

/**
 * main - ELF file header reader
 * @argc: number of arguments
 * @argv: array of strings
 *
 * Return: 0 if success, errno otherwise
 */
int main(int argc, char **argv)
{
	int fread_rtn, buf_size = 64;
	FILE *fptr;
	unsigned char buf[buf_size];

	if (argc < 2)  /* too few arguments */
	{
		printf("argc: %d\n", argc);
		return (0);
	}
	memset(buf, '\0', buf_size);

	fptr = fopen(argv[1], "rb");  /* opens file ptr to file w/read-binary */
	fread_rtn = fread(buf, 1, buf_size, fptr);  /* reads 64 bytes into buf */
	if (fread_rtn < 64)
	{
		perror("fread_rtn < 0");
		fclose(fptr);
		return (0);
	}
	fclose(fptr);

	/* ELF Check */
	if (!(buf[0] == 0x7f) && (buf[1] == 0x45) &&
	(buf[2] == 0x4c) && (buf[3] == 0x46))
	{
		fprintf(stderr, "File is not an ELF file.\n");
		return (0);
	}

	return (elfread(buf));
}

/**
 * elfread - replicated readelf
 * @buf: buffer of first 64 bytes
 *
 * Return: 0 if successful, otherwise error
 */
int elfread(unsigned char buf[])
{
	/* print Elf Header */
	printf("ELF Header:\n");

	firstFour(buf);
	OSABI(buf);
	ABIv(buf);
	Type(buf);
	Machine(buf);
	thirdSet(buf);
	fourthSet(buf);
	fifthSet(buf);
	sixthSet(buf);
	seventhSet(buf);

	return (0);
}


/**
 * buildSixFour - builds a uint64_t from individual bytes in little-endian
 * @entry: pointer to a uint64_t to be initiated
 * @buf: 64-byte buffer from target file
 * @tracker: current index of code area
 */
void buildSixFour(uint64_t *entry, unsigned char buf[], int tracker)
{
	*entry =  (uint64_t)buf[tracker] |
	((uint64_t)buf[tracker + 1] << 8)  |
	((uint64_t)buf[tracker + 2] << 16) |
	((uint64_t)buf[tracker + 3] << 24) |
	((uint64_t)buf[tracker + 4] << 32) |
	((uint64_t)buf[tracker + 5] << 40) |
	((uint64_t)buf[tracker + 6] << 48) |
	((uint64_t)buf[tracker + 7] << 56);
}

/**
 * buildThreeTwo - builds a uint32_t from individual bytesin little-endian
 * @entry: pointer to a uint64_t to be initiated
 * @buf: 32-byte buffer from target file
 * @tracker: current index of code area
 */
void buildThreeTwo(uint32_t *entry, unsigned char buf[], int tracker)
{
	*entry =  (uint32_t)buf[tracker] |
	((uint32_t)buf[tracker + 1] << 8)  |
	((uint32_t)buf[tracker + 2] << 16) |
	((uint32_t)buf[tracker + 3] << 24);
}

/**
 * buildOneSix - builds a uint32_t from individual bytesin little-endian
 * @entry: pointer to a uint64_t to be initiated
 * @buf: 32-byte buffer from target file
 * @tracker: current index of code area
 */
void buildOneSix(uint16_t *entry, unsigned char buf[], int tracker)
{
	*entry =  (uint16_t)buf[tracker] |
	((uint16_t)buf[tracker + 1] << 8);
}
