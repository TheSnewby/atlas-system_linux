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
int heap_rw(int pid, long mem_begin, long mem_end, char *find, char *replace)
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
		return (-1);
	}

	for (addr = mem_begin; addr <= mem_end; addr += word_size)
	{
		word = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
		if (word == -1)
		{
			fprintf(stderr, "Error in ptrace peek: ");
			perror(NULL);
			break;
		}
		else
		{
			memcpy(data, &word, word_size);
			data[word_size - 1] = '\0';
			printf("word: %s\n", data);  /* for debugging */

			if (strcmp(find, data) == 0)
			{
				printf("find found!\n");
				memcpy(&word, replace, word_size);  /* store value of replace */

				ptrace_rtn = ptrace(PTRACE_POKEDATA, pid, addr, word);  /* should (long)replace? */
				if (ptrace_rtn == -1)
				{
					fprintf(stderr, "Error in ptrace poke: ");
					perror(NULL);
				}
			}
			else if (strstr(find, data) != NULL)  /* find in word */
			{
				printf("find in word!\nFigure out what to do.\n");
			}
			/* have a case for *find being found in across multiple words? */
		}
	}

	ptrace_rtn = ptrace(PTRACE_DETACH, pid, 0, 0);  /* consider adding this to all errors */
	if (ptrace_rtn == 0)
		printf("Detached Successfully!\n");
	return (0);
}

// int main(void)
// {
// 	// heap_rw(610, "5570452c3000", "5570452e4000", "hi", "goodbye");
// 	return 0;
// }
