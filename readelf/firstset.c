#include "main.h"

/**
 * firstFour - prints magic number, class, data, and ELF Version
 * @buf: buffer of first 64 bytes
 */
void firstFour(unsigned char buf[])
{
	int i;

	/* print Magic Number */
	printf("  Magic:   ");
	for (i = 0; i < 16; i++)
		printf("%02x ", buf[i]);
	printf("\n");

	/* Class */
	printf("  Class:                             ");
	if (buf[4] == 1)
		printf("ELF32\n");
	else if (buf[4] == 2)
		printf("ELF64\n");

	/* Data */
	printf("  Data:                              ");
	if (buf[5] == 1)
		printf("2's complement, little endian\n");
	else if (buf[5] == 2)
		printf("2's complement, big endian\n");

	/* ELF Version */
	printf("  Version:                           1 (current)\n");
}

/**
 * OSABI - prints the OS/ABI information
 * @buf: buffer of first 64 bytes
 */
void OSABI(unsigned char buf[])
{
	/* OS/ABI Top Half */
	printf("  OS/ABI:                            ");
	switch (buf[7])
	{
		case 0:
			printf("UNIX - System V\n");
			break;
		case 1:
			printf("UNIX - HP-UX\n");
			break;
		case 2:
			printf("UNIX - NetBSD\n");
			break;
		case 3:
			printf("UNIX - Linux\n");
			break;
		case 4:
			printf("UNIX - GNU Hurd\n");
			break;
		case 6:
			printf("UNIX - Solaris\n");
			break;
		case 7:
			printf("UNIX - AIX (Monterey)\n");
			break;
		case 8:
			printf("UNIX - IRIX\n");
			break;
		case 9:
			printf("UNIX - FreeBSD\n");
			break;
		default:
			OSABItwo(buf);
			break;
	}
}

/**
 * OSABItwo - continuation of OSABI
 * @buf: buffer of first 64 bytes
 */
void OSABItwo(unsigned char buf[])
{
	/* OS/ABI Bottom Half */
	switch (buf[7])
	{
		case 0x0A:
			printf("UNIX - Tru64\n");
			break;
		case 0x0B:
			printf("UNIX - Novell Modesto\n");
			break;
		case 0x0C:
			printf("UNIX - Open BSD\n");
			break;
		case 0x0D:
			printf("UNIX - OpenVMS\n");
			break;
		case 0x0E:
			printf("UNIX - NonStop Kernel\n");
			break;
		case 0x0F:
			printf("UNIX - AROS\n");
			break;
		case 0x10:
			printf("UNIX - FenixOS\n");
			break;
		case 0x11:
			printf("UNIX - Nuxi CloudABI\n");
			break;
		case 0x12:
			printf("UNIX - Stratus Technologies OpenVOS\n");
			break;
		default:
			printf("<unknown: %x>\n", buf[7]);
			break;
	}
}

/**
 * ABIv - prints the ABI Version
 * @buf: buffer of the first 64 bytes
 */
void ABIv(unsigned char buf[])
{
	/* ABI Version */
	printf("  ABI Version:                       %d\n", buf[8]);
}

/**
 * Type - prints the Type
 * @buf: buffer of the first 64 bytes
 */
void Type(unsigned char buf[])
{
	uint16_t entryOneSix;

	/* TYPE */
	buildOneSix(&entryOneSix, buf, 0x10);
	if (buf[5] == 2)
		entryOneSix = bswap_16(entryOneSix);
	printf("  Type:                              ");
	switch (entryOneSix)
	{
	case 0x00:
		printf("Unknown\n");
		break;
	case 0x01:
		printf("REL (Relocatable file)\n");
		break;
	case 0x02:
		printf("EXEC (Executable file)\n");
		break;
	case 0x03:
		printf("DYN (Shared object file)\n");
		break;
	case 0x04:
		printf("CORE (Core file)\n");
		break;
	case 0xFE00:
		printf("LOOS (Reserved inclusive range)\n");
		break;
	case 0xFEFF:
		printf("HIOS (Operating system specific)\n");
		break;
	case 0xFF00:
		printf("LOPROC (Reserved inclusive range)\n");
		break;
	case 0xFFFF:
		printf("HIPROC (Processor specific)\n");
		break;
	default:
		printf("Default: Unknown\n");
		break;
	}
}
