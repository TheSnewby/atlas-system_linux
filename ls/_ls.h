#ifndef _ls_h
#define _ls_h
#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#define MAX_OPTIONS 2  /* currently only handles -l and -a */

char *_ls(int argc, char **argv);

#endif
