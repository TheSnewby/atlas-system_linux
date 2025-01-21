#include "_ls.h"

/**
 * long_print_dir - handles long format for printing
 * @argc: number of arguments
 * @directory: string of directory
 * @options: options array where [0] = 1 means long (-l) and [1] = all (-a)
 *
 * Return: void
 */
void long_print_dir(int argc, char *directory, int *options)
{
	printf("%d, %s, %d", argc, directory, options[0]);
	return;
}

/**
 * print_dir - prints the passed directory
 * @argc: number of arguments
 * @directory: string of directory
 * @options: options array where [0] = 1 means long (-l) and [1] = all (-a)
 *
 * Return: void
 */
void print_dir(int argc, char *directory, int *options)
{
	struct dirent *entry;
	DIR *dir;

	if (argc == 1)
	{
		dir = opendir(".");
		if (dir == NULL)
			fprintf(stderr, "opendir failure in print_dir\n");
	}
	else
	{
		dir = opendir(directory);
		if (dir == NULL)
			fprintf(stderr, "opendir failure in print_dir for directory: %s\n",
			directory);
	}

	if (options[0] == 0 && options[1] == 0)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_name[0] != '.')
				printf("%s\t", entry->d_name);  /* might need formatting rework? */
		}
		printf("\n");
	}
	else if (options[0] == 0 && options[1] == 1)  /* only -a */
	{
		while ((entry = readdir(dir)) != NULL)  /* prints all of directory */
			printf("%s\t", entry->d_name);  /* might need formatting rework? */
		printf("\n");
	}
	else if(options[0] == 1 && options[1] == 0)
		long_print_dir(argc, directory, options);
	else
		long_print_dir(argc, directory, options);
	if (closedir(dir) < 0)
		fprintf(stderr, "closedir failure in print_dir\n");
}

/**
 * parse_options - accumulates all options
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: int array of options where [0] = 1 is -l and [1] = 1 is -a,
 * (add more as needed). NULL if no options.
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
	int i, print_count = 0;
	int *options;
	char directory[PATH_MAX];

	options = parse_options(argc, argv);

	if (argc == 1)  /* default no arguments */
		print_dir(argc, ".", options);
	else  /* iterate through arguments and print dirs */
	{
		for (i = 1; i < argc; i++)
		{
			if (argv[i][0] != '-')
			{
				sprintf(directory, "%s%s", "./", argv[i]);
				print_dir(argc, directory, options);
				sprintf(directory, "./");  /* reset directory, memset not allowed */
				print_count++;
			}
			if (print_count == 0)
				print_dir(argc, ".", options);
		}
	}
	return (0);
}

