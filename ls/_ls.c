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
 * parse_options - accumulates all options after possible directory
 * @argc: number of arguments
 * @argv: array of arguments
 * @option_start_index: index of argv at which options start
 *
 * Return: string/array of options (integer)? , NULL if no options?
 */
char *parse_options(int argc, char **argv, int options_start_index)
{
	/* feel free to change or remove this preliminary function */
	return (NULL);
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
	if (getcwd(cwd, sizeof(cwd)) == NULL)  /* might remove because '.' works */
	{
		printf("getcwd is NULL\n");  /* DEBUG */
		return (NULL);
	}
	// printf("cwd is: %s\n", cwd);  /* DEBUG */
	/* or set cwd equal to "." */

	/* FIGURE OUT DIRECTORY */
	/* figure out whether 2nd argument is a directory */
	/* concat the directory to "./" */
	
	if (argc >= 3) /* this section needs to be redone, supposed to handle more than one argument */
	{
		if(((dir = opendir(strcat("./", argv[2]))) != NULL))  /* check if third argument is a directory */
		{  /* directories can be called like this: ls subdir and ls subdir/ */
			option_start_index = 3;
			parse_options(argc, argv, option_start_index);
		}
		else
			{
				if((dir = opendir(".")) != NULL)
				{
					option_start_index = 2;
					parse_options(argc, argv, option_start_index);
				}
			}
	}
	else
		if ((dir = opendir(".")) == NULL)
		{
			printf("opendir failure\n");
			return (NULL);
		}

	
	while((entry = readdir(dir)) != NULL)
		printf("%s\n", entry->d_name);
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
