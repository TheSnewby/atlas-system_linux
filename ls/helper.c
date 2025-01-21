#include "_ls.h"

/**
 * is_file - returns whether a directory is actually a file
 * @directory: string of filepath
 * @program_name: argv[0]
 *
 * Return: 0 if not a file, 1 if a file
 */
int is_file(char *directory, char *program_name)
{
	struct stat buf;
	int lstat_rtn = 0;

	lstat_rtn = lstat(directory, &buf);
	if (lstat_rtn == -1)
	{
		fprintf(stderr, "%s: ", program_name);  /* insert proper error msg */
		perror(NULL);
		exit(errno);  /* not sure if correct */
	}
	return (S_ISREG(buf.st_mode));
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
		if(fp[i] == '/')
			break;
	}
	dir_size = i;
	dir = (char *)malloc((dir_size + 1) * sizeof(char));
	if (dir)
	{
		fprintf(stderr, "%s: ", program_name);  /* insert proper error msg */
		perror(NULL);
		exit(errno);  /* not sure if correct */
	}
	for (i = 0; i < dir_size; i++)
	{
		dir[i] = fp[i];
	}
	dir[dir_size] = '\0';
	return (dir);
}

/**
 * get_file_of_path - grabs filename of path in file_path
 * @file_path: string of file_path
 *
 * Return: string of file name
 */
char *get_file_of_path(char *fp, char *program_name)
{
	int i = 0, fp_size, file_name_size;
	char *file_name = NULL;

	while (fp[i] != '\0')
		i++;
	fp_size = i;

	for (i = fp_size; i >= 0; i--)
	{
		if(fp[i] == '/')
			break;
	}

	file_name_size = fp_size - i;
	file_name = (char *)malloc((file_name_size + 1) * sizeof(char));
	if (file_name)
	{
		fprintf(stderr, "%s: ", program_name);  /* insert proper error msg */
		perror(NULL);
		exit(errno);  /* not sure if correct */
	}

	for (i = i; i < fp_size; i++)
		file_name[i] = fp[i];
	file_name[file_name_size] = '\0';

	return (file_name);
}
