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
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>

#define MAX_OPTIONS 4  /* currently handles -l, -a, -A, -1 */

int is_file(char *);
int is_dir(char *);
char *get_dir_of_path(char *, char*);
char *get_file_of_path(char *, char *);
int _strcmp(char *, char*);
char *_memcpy(char *, char *, int);
char *_strcpy(char *, char *);
char *remove_dot_slash(char *, char *);
void get_perms(struct stat, char *);
void char_replacer(char *, char, char);
char *adjust_long_time(char *);
void print_error(int, char*, char *, int, char *, char *);

#endif
