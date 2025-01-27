#include "_ls.h"

/**
 * long_print_dir - handles long format by printing the non \n string of info
 * @argc: number of arguments
 * @directory: string of directory
 *
 * Return: void
 */
void long_print_dir(char *directory)
{
	struct stat buf;
	struct passwd *pwd = NULL;
	struct group *group = NULL;
	char uname[32], gname[32], *last_mod = NULL;
	char error_message[256], perms[11] = "----------";

	if (lstat(directory, &buf) == -1) /* check for lstat failure */
	{
		/* buffer for error message */
		sprintf(error_message, "ls: cannot access %s", directory);
		perror(error_message);
		return;
	}

	get_perms(buf, perms);

	pwd = getpwuid(buf.st_uid);  /* get user name from pwd struct */
	if (pwd)
		sprintf(uname, "%s", pwd->pw_name);
	else
		sprintf(uname, "%u", buf.st_uid);

	group = getgrgid(buf.st_gid);  /* get group name from group struct */
	if (group)
		sprintf(gname, "%s", group->gr_name);
	else
		sprintf(uname, "%u", buf.st_uid);

	last_mod = ctime(&buf.st_mtime);  /* get time last modified from ctime */
	char_replacer(last_mod, '\n', '\0');

	last_mod = adjust_long_time(last_mod);

	printf("%s %lu %s %s %5ld %s ", perms, buf.st_nlink, uname, gname, buf.st_size, last_mod);

	/* currently prints out year, whereas ls on shows year if it is different */
	/* ...compared to the current year*/
	/* ls replaces the hour:minutes with the year */
}

/**
 * print_dir - prints the passed path
 * @argc: number of arguments
 * @path: string of path
 * @options: options array where [0] = 1 means long (-l) and [1] = all (-a),
 * [2] = 1 is -A, and [3] = 1 is -1
 *
 * Return: void
 */
void print_dir(int argc, char *path, int *options, char *program_name)
{
	struct dirent *entry;
	DIR *dir;
	char *file_name = NULL, original_path[PATH_MAX];
	int op_long = options[0], op_all = options[1];
	int op_almost = options[2], op_one = options[3];

	if (op_long)
		op_one = 1;  /* use vertical printing for long printing */

	remove_dot_slash(original_path, path);  /* used in error messages */
	if (argc == 1)
	{
		dir = opendir(".");
		if (dir == NULL)
		{
			print_error(1, program_name, path, errno, NULL, NULL);
			return;
		}
	}
	else
	{
		if (is_file(path))
		{
			file_name = get_file_of_path(path, program_name);
			path = get_dir_of_path(path, program_name);
		}
		dir = opendir(path);
		if (dir == NULL)
		{
			print_error(1, program_name, path, errno, NULL, NULL);
			return;
		}
	}
	/* consider refactoring this next section to another function */
	while ((entry = readdir(dir)) != NULL)
	{
		if (op_almost)
			if ((!_strcmp(entry->d_name, ".")) || (!_strcmp(entry->d_name, "..")))  /* what about if file? */
			{
				if (file_name)
				{
					free(file_name);
					free(path);
				}
				continue;
			}
		if (!op_all)
			if (entry->d_name[0] == '.')
			{
				if (file_name)
				{
					free(file_name);
					free(path);
				}
				continue;
			}

		if (!file_name)  /* path isn't a file */
		{
			if (op_long)
				long_print_dir(entry->d_name);
			if (op_one == 0)
				printf("%s\t", entry->d_name);
			else
				printf("%s\n", entry->d_name);
		}
		else  /* path is a file */
		{
			if (op_long)
				long_print_dir(entry->d_name);
			if (_strcmp(file_name, entry->d_name) == 0)
			{
				printf("%s", original_path);
				if (op_one == 0)
					printf("\t");
				else
					printf("\n");
			}
		}
	}
	if (op_one == 0)
		printf("\n");

	if (closedir(dir) < 0)
		print_error(2, program_name, path, errno, NULL, NULL);
	if (file_name)
	{
		free(file_name);
		free(path);
	}
}

/**
 * parse_options - accumulates all options
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: int array of options where [0] = 1 is -l and [1] = 1 is -a,
 * [2] = 1 is -A, and [3] = 1 is -1. Default array of zeros.
 */
int *parse_options(int argc, char **argv)
{
	int i, j;
	static int options[MAX_OPTIONS] = {0};  /* see Return above */

	for (i = 1; i < argc; i++)  /* iterate through argvs */
	{
		if (argv[i][0] == '-')  /* if option */
		{
			for (j = 1; argv[i][j] != '\0'; j++)  /* iterate through chars */
			{
				switch (argv[i][j])
				{
				case 'l':
					options[0] = 1;  /* sets long (-l) option */
					break;
				case 'a':
					options[1] = 1;  /* sets all (-a) option */
					break;
				case 'A':
					options[2] = 1;  /* sets almost all (-A) option */
					break;
				case '1':
					options[3] = 1;  /* sets vertical (-1) option*/
					break;
				default:  /* if unrecognized, print error */
					fprintf(stderr, "ls: invalid option -- %c\n", argv[i][j]);
				}
			}
		}
	}

	return (options);
}

/**
 * main - main entry
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: 0
 */
int main(int argc, char **argv)
{
	int i, print_count = 0, dir_count = 0;
	int *options;
	char directory[PATH_MAX];

	options = parse_options(argc, argv);

	for (i = 1; i < argc; i++)  /* counts number of directories */
	{
		sprintf(directory, "%s%s", "./", argv[i]);
		if ((argv[i][0] != '-') && (is_dir(directory)))
			dir_count++;
		sprintf(directory, "./");  /* reset directory, memset not allowed */
	}
	if (!dir_count)  /* default no arguments */
		print_dir(argc, ".", options, argv[0]);
	else /* iterate through arguments and print dirs */
	{
		for (i = 1; i < argc; i++)
		{
			if (argv[i][0] != '-')
			{
				sprintf(directory, "%s%s", "./", argv[i]);
				/* prints directory if multiple directories, otherwise doesn't */
				if ((dir_count > 1) && (is_dir(directory)))
				{
					if (print_count)
						printf("\n");
					printf("%s:\n", argv[i]);
				}
				print_dir(argc, directory, options, argv[0]);
				sprintf(directory, "./");  /* reset directory, memset not allowed */
				print_count++;
			}
			if (print_count == 0)
				print_dir(argc, ".", options, argv[0]);
		}
	}
	return (0);
}
