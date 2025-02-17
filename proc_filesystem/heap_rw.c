#include <stdio.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>

/**
 * heap_rw - opens a memory address, adjusted permissions, and replaces a value
 * @pid: process id
 * @mem_begin: beginning of memory range of heap in PID
 * @mem_end: end of memory range of heap in PID
 *
 * Return: 0 is successful, other if error
 */
int heap_rw(int pid, int mem_begin, int mem_end, char *find, char *replace)
{
	int ptrace_rtn = 0;
	size_t word_size = sizeof(long);
	char data[word_size];
	long word;
	unsigned int addr;

	printf("find: %s\nreplace: %s\n", find, replace);

	ptrace_rtn = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (ptrace_rtn == -1)
	{
		fprintf(stderr, "Error in ptrace attach: ");
		perror(NULL);
	}

	for (addr = (long)mem_begin; addr <= (long)mem_end; addr += word_size)
	{
		word = ptrace(PTRACE_PEEKDATA, pid, mem_begin, NULL);
		if (word == -1)
		{
			fprintf(stderr, "Error in ptrace peek: ");
			perror(NULL);
			break;
		}
		else
		{
			printf("word: %s\n", (char *) word);
			if (strcmp(find, (char *)word) == 0)
			{
				printf("find found!\n");
				ptrace_rtn = ptrace(PTRACE_POKEDATA, pid, addr, replace);  /* should (long)replace? */
				if (ptrace_rtn == -1)
				{
					fprintf(stderr, "Error in ptrace poke: ");
					perror(NULL);
				}
			}
			else if (strstr(find, (char *) word) != NULL)  /* find in word */
			{
				printf("find in word!\nFigure out what to do.\n");
			}
			/* have a case for *find being found in across multiple words? */
		}
	}

	ptrace_rtn = (PTRACE_DETACH, pid, 0, 0);  /* consider adding this to all errors */
	if (ptrace_rtn == 0)
		printf("Detached Successfully!\n");
	return (0);
}

// int main(void)
// {
// 	heap_rw(610, "5570452c3000", "5570452e4000", "hi", "goodbye");
// 	return 0;
// }
