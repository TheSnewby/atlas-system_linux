#ifndef main_h
#define main_h
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>
#include <byteswap.h>

/* proper functions*/
int elfread(unsigned char buf[]);
void firstFour(unsigned char buf[]);
void OSABI(unsigned char buf[]);
void OSABItwo(unsigned char buf[]);
void ABIv(unsigned char buf[]);
void Type(unsigned char buf[]);
void Machine(unsigned char buf[]);
void Machinetwo(uint16_t);
void Machinethree(uint16_t);
void Machinefour(uint16_t);
void Machinefive(uint16_t);
void Machinesix(uint16_t);
void Machineseven(uint16_t);
void Machineeight(uint16_t);
void thirdSet(unsigned char buf[]);
void fourthSet(unsigned char buf[]);
void fifthSet(unsigned char buf[]);
void sixthSet(unsigned char buf[]);
void seventhSet(unsigned char buf[]);

/* utility functions */
void buildSixFour(uint64_t *entry, unsigned char buf[], int tracker);
void buildThreeTwo(uint32_t *entry, unsigned char buf[], int tracker);
void buildOneSix(uint16_t *entry, unsigned char buf[], int tracker);

#endif
