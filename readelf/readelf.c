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
	int fread_rtn, buf_size = 64, i, isSixFour = 0;
	FILE *fptr;
	unsigned char buf[buf_size];  // **strtol_endptr = NULL;
	uint16_t entryOneSix;
	uint32_t entryThreeTwo;
	uint64_t entrySixFour;

	memset(buf, '\0', buf_size);
	if (argc < 2)  /* too few arguments */
	{
		printf("argc: %d\n", argc);
		return (0);
	}


	fptr = fopen(argv[1], "rb");  /* opens file pointer to file with read-binary */
	fread_rtn = fread(buf, 1, buf_size, fptr);  /* reads 64 bytes into buf */
	if (fread_rtn < 64)
	{
		perror("fread_rtn < 0");
		fclose(fptr);
		return (0);
	}
	fclose(fptr);

	/* ELF Check */
	if (!(buf[0] == 0x7f) && (buf[1] == 0x45) && (buf[2] == 0x4c) && (buf[3] == 0x46))
	{
		fprintf(stderr, "File is not an ELF file.\n");
		return (0);
	}

	/* print Elf Header */
	printf("ELF Header:\n");

	/* print Magic Number */
	printf("  Magic:   ");
	for (i = 0; i < 16; i++)
	{
		printf("%02x ", buf[i]);
		// if (i == 0)
		// 	printf("%02x", buf[i]);
		// else
		// 	printf(" %02x", buf[i]);
	}
	printf("\n");

	/* Class */
	printf("  Class:                             ");
	if (buf[4] == 1)
		printf("ELF32\n");
	else if (buf[4] == 2)
	{
		printf("ELF64\n");
		isSixFour = 1;
	}

	/* Data */
	printf("  Data:                              ");
	if (buf[5] == 1)
		printf("2's complement, little endian\n");
	else if (buf[5] == 2)
		printf("2's complement, big endian\n");

	/* ELF Version */
	printf("  Version:                           1 (current)\n");

	/* OS/ABI */
	printf("  OS/ABI:                            ");
	switch(buf[7])
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

	/* ABI Version */
	printf("  ABI Version:                       %d\n", buf[8]);

	/* TYPE */
	buildOneSix(&entryOneSix, buf, 0x10);
	// printf("buildOneSix: 0x04%x\n", entryOneSix);
	if (buf[5] == 2)
		entryOneSix = bswap_16(entryOneSix);
	// printf("bswap_16: 0x04%x\n", entryOneSix);
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

	/* Machine */
	buildOneSix(&entryOneSix, buf, 0x12);
	if (buf[5] == 2)
		entryOneSix = bswap_16(entryOneSix);
	printf("  Machine:                           ");
	switch(entryOneSix)
	{
		case 0x00:
			printf("No specific instruction set\n");
			break;
		case 0x01:
			printf("AT&T WE 32100\n");
			break;
		case 0x02:
			printf("Sparc\n");
			break;
		case 0x03:
			printf("Intel 80386\n");  /* or x86 */
			break;
		case 0x04:
			printf("Motorola 68000 (M68k)\n");
			break;
		case 0x05:
			printf("Motorola 88000 (M88k)\n");
			break;
		case 0x06:
			printf("Intel MCU\n");
			break;
		case 0x07:
			printf("Intel 80860\n");
			break;
		case 0x08:
			printf("MIPS\n");
			break;
		case 0x09:
			printf("IBM System/370\n");
			break;
		case 0x0A:
			printf("MIPS RS3000 Little-endian\n");
			break;
		case 0x0F:
			printf("Hewlett-Packard PA-RISC\n");
			break;
		case 0x13:
			printf("Intel 80960\n");
			break;
		case 0x14:
			printf("PowerPC\n");
			break;
		case 0x15:
			printf("PowerPc (64-bit)\n");
			break;
		case 0x16:
			printf("S390, including S390x\n");
			break;
		case 0x17:
			printf("IBM SPU/SPC\n");
			break;
		case 0x24:
			printf("NEC V800\n");
			break;
		case 0x25:
			printf("Fujitsu FR20\n");
			break;
		case 0x26:
			printf("TRW RH-32\n");
			break;
		case 0x27:
			printf("Motorola RCE\n");
			break;
		case 0x28:
			printf("Arm (up to Armv7/AArc32\n");
			break;
		case 0x29:
			printf("Digital Alpha\n");
			break;
		case 0x2A:
			printf("SuperH\n");
			break;
		case 0x2B:
			printf("SPARC Version 9\n");
			break;
		case 0x2C:
			printf("Siemens TriCore embeeded processor\n");
			break;
		case 0x2D:
			printf("Argonaut RISC Core\n");
			break;
		case 0x2E:
			printf("Hitachi H8/300\n");
			break;
		case 0x2F:
			printf("Hitachi H8/300H\n");
			break;
		case 0x30:
			printf("Hitachi H8S\n");
			break;
		case 0x31:
			printf("Hitachi H8/500\n");
			break;
		case 0x32:
			printf("IA-64\n");
			break;
		case 0x33:
			printf("Stanford MIPS-X\n");
			break;
		case 0x34:
			printf("Motorola ColdFire\n");
			break;
		case 0x35:
			printf("Motorola M68HC12\n");
			break;
		case 0x36:
			printf("Fujitsu MMA Multimedia Accelerator\n");
			break;
		case 0x37:
			printf("Siemens PCP\n");
			break;
		case 0x38:
			printf("Sony nCPU embedded RISC processor\n");
			break;
		case 0x39:
			printf("Denso NDR1 microprocessor\n");
			break;
		case 0x3A:
			printf("Motorola Star*Core processor\n");
			break;
		case 0x3B:
			printf("Toyota ME16 processor\n");
			break;
		case 0x3C:
			printf("STMicroelectronics ST100 processor\n");
			break;
		case 0x3D:
			printf("Advanced Logic Corp. TinyJ embeded processor family\n");
			break;
		case 0x3E:
			printf("Advanced Micro Devices x86-64\n");
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
	}

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

	// for (i = 0; i < 64; i++)  /* -- DEBUG -- */
	// {
	// 	if (i % 16 == 0 && i != 0)
	// 		printf("\n");
	// 	if (i % 16)
	// 		printf(" ");
	// 	printf("%02x", buf[i]);
	// }
	// printf("\n");
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
	*entry =  (uint64_t)buf[tracker]|
	((uint64_t)buf[tracker+1] << 8)  |
	((uint64_t)buf[tracker+2] << 16) |
	((uint64_t)buf[tracker+3] << 24) |
	((uint64_t)buf[tracker+4] << 32) |
	((uint64_t)buf[tracker+5] << 40) |
	((uint64_t)buf[tracker+6] << 48) |
	((uint64_t)buf[tracker+7] << 56);

	// printf("entrySixFour (hex): 0x%016lx\n", *entry);
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
	((uint32_t)buf[tracker+1] << 8)  |
	((uint32_t)buf[tracker+2] << 16) |
	((uint32_t)buf[tracker+3] << 24);
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
	((uint16_t)buf[tracker+1] << 8);
}