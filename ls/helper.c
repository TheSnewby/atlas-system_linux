#include "_ls.h"

/**
 * get_perms - get the permissions of a file path
 * @buf: lstat return of file path
 * @perms: permissions buffer
 *
 * Return: void, perms is modified here
 */
void get_perms(struct stat buf, char *perms)
{
	_memcpy(perms, "----------", 10);  /* default setting */

	/* set file type, default is regular file '-' set in calling function*/
	if (S_ISDIR(buf.st_mode))
		perms[0] = 'd';
	else if (S_ISSOCK(buf.st_mode))
		perms[0] = 's';
	else if (S_ISLNK(buf.st_mode))
		perms[0] = 'l';
	else if (S_ISBLK(buf.st_mode))
		perms[0]= 'b';
	else if (S_ISCHR(buf.st_mode))
		perms[0] = 'c';
	else if (S_ISFIFO(buf.st_mode))
		perms[0] = 'p';

	/* set USR, GRP, and OTH permissions, default '-' set in calling function */
	if (buf.st_mode & S_IRUSR)
		perms[1] = 'r';
	if (buf.st_mode & S_IWUSR)
		perms[2] = 'w';
	if (buf.st_mode & S_IXUSR)
		perms[3] = 'x';
	if (buf.st_mode & S_IRGRP)
		perms[4] = 'r';
	if (buf.st_mode & S_IWGRP)
		perms[5] = 'w';
	if (buf.st_mode & S_IXGRP)
		perms[6] = 'x';
	if (buf.st_mode & S_IROTH)
		perms[7] = 'r';
	if (buf.st_mode & S_IWOTH)
		perms[8] = 'w';
	if (buf.st_mode & S_IXOTH)
		perms[9] = 'x';
}

/**
 * is_dir - checks if the path is a directory
 * @directory: string of path being checked
 *
 * Return: 1 if directory, otherwise 0
 */
int is_dir(char *directory)
{
    struct stat buf; /* struct to hold file info */

    if (lstat(directory, &buf) == -1) /* check for lstat failure */
		return (0);

    return (S_ISDIR(buf.st_mode)); /* actual directory check */
}


/**
 * is_file - checks if the path is a file
 * @directory: string of path being checked
 *
 * Return: 1 if standard file, otherwise 0
 */
int is_file(char *directory)
{
    struct stat buf; /* holds file info */

    if (lstat(directory, &buf) == -1) /* lsat failure check */
		return (0);

    return (S_ISREG(buf.st_mode)); /* actual file check */
}

/**
 * get_dir_of_path - grabs directory location of a file in a file_path
 * @file_path: string of file_path
 *
 * Return: string of directory path
 */
char *get_dir_of_path(char *fp, char *program_name)
{
	int i = 0, fp_size, dir_size;
	char *dir = NULL;

	while (fp[i] != '\0')
		i++;
	fp_size = i;

	for (i = fp_size; i >= 0; i--)
	{
		if (fp[i] == '/')
			break;
	}
	dir_size = i;
	char error_message[256];

	dir = malloc((dir_size + 1) * sizeof(char)); /* removed (char *) */
	if (!dir) /* now checks for failure, not success */
	{
		sprintf(error_message, "%s: ", program_name); /* NEEDS ERROR MESSAGE */
		perror(error_message);
		exit(errno);
	}
	for (i = 0; i < dir_size; i++)
	{
		dir[i] = fp[i];
	}
	dir[dir_size] = '\0';

	return (dir);
}

/**
 * get_file_of_path - grabs file name of path in file_path
 * @fp: full file path
 * @program_name: name of program (argv[0])
 *
 * Return: pointer to string of file name
 */
char *get_file_of_path(char *fp, char *program_name)
{
	int i, start;
	int fp_size = 0;	  /* stores full path length */
	int slash_index = -1; /* stores position of last slash */
	char error_message[256];

	while (fp[fp_size] != '\0') /* get length of input path */
		fp_size++;

	for (i = fp_size; i >= 0; i--) /* find last slash in path */
	{
		if (fp[i] == '/')
		{
			slash_index = i;
			break;
		}
	}
	/* position of file name in path */
	if (slash_index == -1) /* no slash */
        start = 0;
	else /* slash found */
	{
        start = slash_index + 1;
	}
	int file_name_size = fp_size - start;

	char *file_name = malloc(file_name_size + 1); /* name + /0 */
	if (!file_name)
	{
        sprintf(error_message, "%s: Memory allocation failed in get_file_of_path", program_name);
        perror(error_message);
        exit(errno);
    }
	/* copy file name into allocated memory */
	for (i = 0; i < file_name_size; i++)
		file_name[i] = fp[start + i];
	file_name[file_name_size] = '\0';

    if (file_name_size == 0)
    {
        char error_msg[] = "ls: missing file operand\n";
        write(STDOUT_FILENO, error_msg, sizeof(error_msg) - 1);
        exit(EXIT_FAILURE);
    }

	return (file_name);
}
