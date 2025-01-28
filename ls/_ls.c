#include "_ls.h"

/**
 * long_print - handles long format by printing the non \n string of info
 * @argc: number of arguments
 * @path: string of path
 *
 * Return: void
 */
void long_print(char *path)
{
	struct stat buf;
	struct passwd *pwd = NULL;
	struct group *group = NULL;
	char uname[32], gname[32], time_str[32], perms[11] = "----------";
	char error_message[256];
	time_t current_time = time(NULL);
	struct tm *mod_time = NULL, *curr_time = localtime(&current_time);
	char *file_name = path;

	for (char *p = path; *p; p++)
	{
		if (*p == '/')
			file_name = p + 1;
	}

	if (lstat(path, &buf) == -1) /* check for lstat failure */
	{
		/* buffer for error message */
		sprintf(error_message, "ls: cannot access %s", file_name);
		perror(error_message);
		return;
	}

	get_perms(buf, perms);

	pwd = getpwuid(buf.st_uid); /* get user name from pwd struct */
	if (pwd)
		sprintf(uname, "%s", pwd->pw_name);
	else
		sprintf(uname, "%u", buf.st_uid);

	group = getgrgid(buf.st_gid); /* get group name from group struct */
	if (group)
		sprintf(gname, "%s", group->gr_name);
	else
		sprintf(uname, "%u", buf.st_uid);

	mod_time = localtime(&buf.st_mtime); /* get last update time */

	if (mod_time->tm_year == curr_time->tm_year) /* if same year */
	{
		sprintf(time_str, "%s %2d %02d:%02d",
				get_month_name(mod_time->tm_mon),
				mod_time->tm_mday,
				mod_time->tm_hour,
				mod_time->tm_min);
	}
	else /* if different year */
	{
		sprintf(time_str, "%s %2d  %d",
				get_month_name(mod_time->tm_mon),
				mod_time->tm_mday,
				mod_time->tm_year + 1900);
	}

	printf("%s %lu %s %s %5ld %s %s\n",
			perms, buf.st_nlink, uname, gname, buf.st_size, time_str, file_name);
}

/**
 * print_dir - prints the passed path
 * @path: string of path
 * @options: options array where [0] = 1 means long (-l) and [1] = all (-a),
 * [2] = 1 is -A, and [3] = 1 is -1
 * @program_name: name of compiled program
 *
 * Return: void
 */
void print_dir(char *path, int *options, char *program_name)
{
	struct dirent *entry;
	DIR *dir;
	char long_path[PATH_MAX];
	int op_long = options[0], op_all = options[1], op_almost = options[2];
	char *clean_path = path;

	if (path[0] == '.' && path[1] == '/') /* if string starts with ./ */
		clean_path = path + 2;

	dir = opendir(path);
	if (dir == NULL)
	{
		print_error(1, program_name, clean_path, errno, NULL, NULL);
		return;
	}

	while ((entry = readdir(dir)) != NULL)

	/* consider refactoring this next section to another function */
	while ((entry = readdir(dir)) != NULL)
	{
		if (!op_all && entry->d_name[0] == '.') /* skip hidden files unless -a */
			continue;

		if (op_almost &&
			((entry->d_name[0] != '.') || (entry->d_name[1] != '\0') ||
			 (entry->d_name[0] != '.') || (entry->d_name[1] != '.') &&
			 (entry->d_name[2] != '\0')))
			continue;

		sprintf(long_path, "%s/%s", path, entry->d_name);
		if (op_long)
			long_print(long_path);
		else
			printf("%s\n", entry->d_name);
	}

	closedir(dir);
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
	static int options[MAX_OPTIONS] = {0}; /* see Return above */

	for (i = 1; i < argc; i++) /* iterate through argvs */
	{
		if (argv[i][0] == '-') /* if option */
		{
			for (j = 1; argv[i][j] != '\0'; j++) /* iterate through chars */
			{
				switch (argv[i][j])
				{
				case 'l':
					options[0] = 1; /* sets long (-l) option */
					break;
				case 'a':
					options[1] = 1; /* sets all (-a) option */
					break;
				case 'A':
					options[2] = 1; /* sets almost all (-A) option */
					break;
				case '1':
					options[3] = 1; /* sets vertical (-1) option*/
					break;
				default: /* if unrecognized, print error */
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
	int i, print_count = 0, dir_count = 0, non_dir_count = 0;
	int *options;
	char path[PATH_MAX];

	options = parse_options(argc, argv);

	for (i = 1; i < argc; i++) /* counts number of directories */
	{
		if (argv[i][0] != '-')
		{
			sprintf(path, "%s%s", "./", argv[i]);
			if (is_file(path))
			{
				printf("%s\n", argv[i]);
				non_dir_count++;
				print_count++;
			}
			else if (!is_dir(path))
			{
				print_error(1, argv[0], argv[i], errno, NULL, NULL);
			}
			else
				dir_count++;
		}
	}

	if (non_dir_count > 0 && dir_count > 0) /* if both files & directories */
		printf("\n");

	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-')
		{
			/* prints directory name if multiple directories */
			sprintf(path, "%s%s", "./", argv[i]);
			if (is_dir(path))
			{
				if (dir_count > 1)
				{
					if (print_count > 0)
						printf("\n");
					printf("%s:\n", argv[i]);
				}
				print_dir(path, options, argv[0]);
				print_count++;
			}
		}
	}

	return (0);
}
