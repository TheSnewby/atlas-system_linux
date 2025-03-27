#ifndef hnm_h
#define hnm_h

#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <byteswap.h>

#define BUFF_MAX 1024

/* main functions */
char get_symbol_type(Elf64_Sym *);
int parse_symbol_table(const char *);

/* utility functions */
int is_little_endian(void);
uint64_t swap_uint64(uint64_t, int);
uint32_t swap_uint32(uint32_t, int);
uint16_t swap_uint16(uint16_t, int);


#endif
