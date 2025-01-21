#ifndef _ls_h
#define _ls_h
#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_OPTIONS 4  /* currently only handles -l and -a */

char *_ls(int argc, char **argv);
int is_file(char*, char*);
char *get_dir_of_path(char *, char*);
char *get_file_of_path(char *, char *);

#endif
