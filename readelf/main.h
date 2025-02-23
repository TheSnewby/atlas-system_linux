#ifndef main_h
#define main_h
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>

void buildSixFour(uint64_t *entry, unsigned char buf[], int tracker);
void buildThreeTwo(uint32_t *entry, unsigned char buf[], int tracker);

#endif