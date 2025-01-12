#ifndef getline_h
#define getline_h
#define READ_SIZE 1024
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *_getline(const int fd);

#endif
