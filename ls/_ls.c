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
 */


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
						break;
					case 'a':
						options[1] = 1;  /* sets all (-a) option */
						break;
					default:  /* if unrecognized, print error */
						fprintf(stderr, "ls: invalid option -- %c", argv[i][j]);
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
char *_ls(int argc, char **argv)  /* argv[0] is program */
{
	int i, option_start_index;
	char *cmd = "ls";
	int closedir_rtn;
	char cwd[PATH_MAX];
	DIR *dir;
	struct dirent *entry;
	int *options;
	// DIR *dirs[] = NULL;


	if (argc < 2)
	{
		printf("argc < 2\n");  /* DEBUG */
		return (NULL);
	}

	if (strcmp(argv[1], "hls"))
	{
		printf("Incorrect command for hls.\n");
		return (NULL);
	}

	options = parse_options(argc, argv);
	// printf("cwd is: %s\n", cwd);  /* DEBUG */
	/* or set cwd equal to "." */

	/* FIGURE OUT DIRECTORY */
	/* concat directories to "./" */

	if ((dir = opendir(".")) == NULL)
	{
		printf("opendir failure\n");
		return (NULL);
	}


	while((entry = readdir(dir)) != NULL)  /* prints contents of directory */
		printf("%s\t", entry->d_name);
	printf("\n");
	if (closedir(dir) < 0)
	{
		printf("closedir failure\n");
		return (NULL);
	}


	return (NULL);
}

int main(int argc, char **argv)
{
	_ls(argc, argv);
	return (0);
}
