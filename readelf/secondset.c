#include "main.h"

/**
 * Machine - prints the machine information
 * @buf: buffer containing the first 64 bytes
 */
void Machine(unsigned char buf[])
{
	uint16_t entryOneSix;

	buildOneSix(&entryOneSix, buf, 0x12);
	if (buf[5] == 2)
		entryOneSix = bswap_16(entryOneSix);

	/* Machine */
	printf("  Machine:                           ");
	switch (entryOneSix)
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
		default:
			Machinetwo(entryOneSix);
			break;
	}
}

/**
 * Machinetwo - prints the machine information
 * @entryOneSix: 16-bit code of machine value
 */
void Machinetwo(uint16_t entryOneSix)
{
	switch (entryOneSix)
	{
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
		default:
			Machinethree(entryOneSix);
			break;
	}
}

/**
 * Machinethree - prints the machine information
 * @entryOneSix: 16-bit code of machine value
 */
void Machinethree(uint16_t entryOneSix)
{
	switch (entryOneSix)
	{
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
		default:
			Machinefour(entryOneSix);
			break;
	}
}

/**
 * Machinefour - prints the machine information
 * @entryOneSix: 16-bit code of machine value
 */
void Machinefour(uint16_t entryOneSix)
{
	switch (entryOneSix)
	{
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
		default:
			Machinefive(entryOneSix);
			break;
	}
}

/**
 * Machinefive - prints the machine information
 * @entryOneSix: 16-bit code of machine value
 */
void Machinefive(uint16_t entryOneSix)
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
			Machinesix(entryOneSix);
			break;
	}
}
