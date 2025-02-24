#include "main.h"

/**
 * Machinesix - prints the machine information
 * @entryOneSix: 16-bit code of machine value
 */
void Machinesix(uint16_t entryOneSix)
{
	switch (entryOneSix)
	{
		case 0x3D:
			printf("Advanced Logic Corp. TinyJ embeded processor family\n");
			break;
		case 0x3E:
			printf("Advanced Micro Devices X86-64\n");
			break;
		case 0x3F:
			printf("Sony DSP Processor\n");
			break;
		case 0x40:
			printf("Digital Equipment Corp. PDP-10\n");
			break;
		case 0x41:
			printf("Digital Equipment Corp. PDP-11\n");
			break;
		case 0x42:
			printf("Siemens FX66 microcontroller\n");
			break;
		case 0x43:
			printf("STMicroelectronics ST9+ 8/16 bit microcontroller\n");
			break;
		case 0x44:
			printf("STMicroelectronics ST7 8-bit microcontroller\n");
			break;
		case 0x45:
			printf("Motorola MC68HC16 Microcontroller\n");
			break;
		case 0x46:
			printf("Motorola MC68HC11 Microcontroller\n");
			break;
		case 0x47:
			printf("Motorola MC68HC08 Microcontroller\n");
			break;
		default:
			Machineseven(entryOneSix);
			break;
	}
}

/**
 * Machineseven - prints the machine information
 * @entryOneSix: 16-bit code of machine value
 */
void Machineseven(uint16_t entryOneSix)
{
	switch (entryOneSix)
	{
		case 0x48:
			printf("Motorola MC68HC05 Microcontroller\n");
			break;
		case 0x49:
			printf("Silicon Graphics SVx\n");
			break;
		case 0x4A:
			printf("STMicroelectronics ST19 8-bit microcontroller\n");
			break;
		case 0x4B:
			printf("Digital VAX\n");
			break;
		case 0x4C:
			printf("Axis Communications 32-bit embedded processor\n");
			break;
		case 0x4D:
			printf("Infineon Technologies 32-bit embedded\n");
			break;
		case 0x4E:
			printf("Element 14 64-bit DSP Processor\n");
			break;
		case 0x4F:
			printf("LSI Logic 16-bit DSP Processor\n");
			break;
		case 0x8C:
			printf("TMS320C6000 Family\n");
			break;
		case 0xAF:
			printf("MCST Elbrus e2k\n");
			break;
		case 0xB7:
			printf("Arm 64-bits (Armv8/AArch64)\n");
			break;
		default:
			Machineeight(entryOneSix);
			break;
	}
}

/**
 * Machineeight - prints the machine information
 * @entryOneSix: 16-bit code of machine value
 */
void Machineeight(uint16_t entryOneSix)
{
	switch (entryOneSix)
	{
		case 0xDC:
			printf("Zilog Z80\n");
			break;
		case 0xF3:
			printf("RISC-V\n");
			break;
		case 0xF7:
			printf("Berkeley Packet Filter\n");
			break;
		case 0x101:
			printf("WDC 65C816\n");
			break;
		case 0x102:
			printf("LoongArch\n");
			break;
		default:
			printf("Unknown Type\n");
			break;
	}
}

/**
 * thirdSet - prints Machine version, Entry point, & Start of program headers
 * @buf: buffer containing the first 64 bytes
 */
void thirdSet(unsigned char buf[])
{
	uint64_t entrySixFour;
	uint32_t entryThreeTwo;
	int isSixFour = (buf[4] == 1) ? 0 : 1;
	/* Machine Version */
	printf("  Version:                           0x1\n");  /* consider buf[20]*/

	/* Entry point address */
	printf("  Entry point address:               0x");
	if (isSixFour)
	{
		buildSixFour(&entrySixFour, buf, 0x18);
		if (buf[5] == 2)  /* if big endian, reverse values */
			entrySixFour = bswap_64(entrySixFour);
		printf("%lx\n", entrySixFour);
	}
	else
	{
		buildThreeTwo(&entryThreeTwo, buf, 0x18);
		if (buf[5] == 2)  /* if big endian, reverse values */
			entryThreeTwo = bswap_32(entryThreeTwo);
		printf("%x\n", entryThreeTwo);
	}

	/* Start of program headers */
	printf("  Start of program headers:          ");
	if (isSixFour)
	{
		buildSixFour(&entrySixFour, buf, 0x20);
		if (buf[5] == 2)  /* if big endian, reverse values */
			entrySixFour = bswap_64(entrySixFour);
		printf("%d (bytes into file)\n", (unsigned char)entrySixFour);
	}
	else
	{
		buildThreeTwo(&entryThreeTwo, buf, 0x1C);
		if (buf[5] == 2)  /* if big endian, reverse values */
			entryThreeTwo = bswap_32(entryThreeTwo);
		printf("%d (bytes into file)\n", (unsigned char)entryThreeTwo);
	}
}

/**
 * fourthSet - prints Start of Section Headers & Flags
 * @buf: buffer of the first 64 bytes
 */
void fourthSet(unsigned char buf[])
{
	uint64_t entrySixFour;
	uint32_t entryThreeTwo;
	int isSixFour = (buf[4] == 1) ? 0 : 1;

	/* START OF SECTION HEADERS */
	printf("  Start of section headers:          ");
	if (isSixFour)
	{
		buildSixFour(&entrySixFour, buf, 0x28);
		if (buf[5] == 2)  /* if big endian, reverse values */
			entrySixFour = bswap_64(entrySixFour);
		printf("%ld (bytes into file)\n", entrySixFour);
	}
	else
	{
		buildThreeTwo(&entryThreeTwo, buf, 0x20);
		if (buf[5] == 2)  /* if big endian, reverse values */
			entryThreeTwo = bswap_32(entryThreeTwo);
		printf("%d (bytes into file)\n", entryThreeTwo);
	}

	/* Flags */
	printf("  Flags:                             ");
	buildThreeTwo(&entryThreeTwo, buf, 0x24);
	if (buf[5] == 2)  /* if big endian, reverse values */
		entryThreeTwo = bswap_32(entryThreeTwo);
	printf("0x%x\n", entryThreeTwo);
}
