#include "_ls.h"
/**
 * current ls functionality includes:
 * -la
 * -l -a
 * test -la
 * -la test
 * test/ -la
 * /etc -la
 * test /etc -la
 * test -la /etc
 * 
 * where test is a valid subdirectory and /etc is a known path to a folder.
 * 
 * This means that there should be a collection of directories, and options.
 * Also means that printing should occur for each directory and handle options.
 * By default readdir returns all contents including hidden, so hidden need
 * to be removed if -a is not given.
 * 
 * Currently compiles with:
 * gcc *.c -o hls
 * ./hls to run
 */

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
	int i;
	struct dirent *entry;
	DIR *dir;

	if (argc == 1)
		if ((dir = opendir(".")) == NULL)
			fprintf(stderr, "opendir failure in print_dir\n");
	else
		if ((dir = opendir(directory)) == NULL)
			fprintf(stderr, "opendir failure in print_dir\n");

	if (options[0] == 0 && options[1] == 0)
	{
		while((entry = readdir(dir)) != NULL)
		{
			if (entry->d_name[0] != '.')
				printf("%s\t", entry->d_name);  /* might need formatting rework? */
		}
		printf("\n");
	}
	else if (options[0] == 0 && options[1] == 1)  /* only -a */
	{
		while((entry = readdir(dir)) != NULL)  /* prints all of directory */
			printf("%s\t", entry->d_name);  /* might need formatting rework? */
		printf("\n");
	}

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
	int static options[MAX_OPTIONS] = {0};  /* see Return above */

	for (i = 1; i < argc; i++)  /* iterate through argvs */
	{
		if (argv[i][0] == '-')  /* if option */
		{
			for (j = 1; argv[i][j] != '\0'; j++)  /* iterate through chars */
			{
				switch(argv[i][j]) {
					case 'l':
						options[0] = 1;  /* sets long (-l) option */
						printf("L FOUND!\n");
						break;
					case 'a':
						options[1] = 1;  /* sets all (-a) option */
						printf("A FOUND!\n");
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
 * _ls - recreates ls functionality
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: string of formatted text in pwd
 */
char *_ls(int argc, char **argv)  /* argv[0] is program hls, consider moving all to main */
{
	int i, j;
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
			}
		}
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	_ls(argc, argv);
	return (0);
}
