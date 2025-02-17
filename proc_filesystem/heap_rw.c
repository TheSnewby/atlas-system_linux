#include <stdio.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

/**
 * heap_rw - opens a memory address, adjusted permissions, and replaces a value
 * @pid: process id
 * @mem_begin: beginning of memory range of heap in PID
 * @mem_end: end of memory range of heap in PID
 * @find: string to be replaced
 * @replace: string to do the replacing with
 *
 * Return: 0 is successful, other if error
 */
int heap_rw(int pid, long mem_begin, long mem_end, char *find, char *replace)
{
	int ptrace_rtn = 0;
	size_t word_size = sizeof(long);
	char data[word_size], command[64], status_path[64];;
	long word;
	unsigned int addr;

	printf("find: %s\nreplace: %s\n", find, replace);  /* debug */
	sprintf(command, "cat /proc/%d/maps | grep heap", pid);
	system(command);

	ptrace_rtn = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (ptrace_rtn == -1)
	{
		fprintf(stderr, "Error in ptrace attach: ");
		perror(NULL);
		return (-1);
	}
	waitpid(pid, NULL, 0);  /* ensures process is fully stopped */

	for (addr = mem_begin; addr <= mem_end; addr += word_size)
	{
		sprintf(status_path, "/proc/%d/status", pid); /* status check */
		FILE *status_file = fopen(status_path, "r");
		if (!status_file)
		{
			perror("Process has exited");
			return -1;
		}
		fclose(status_file);

		if (kill(pid, 0) == -1)
			perror("Process is no longer running: ");

		word = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
		if (word == -1)
		{
			fprintf(stderr, "Error in ptrace peek: ");
			perror(NULL);
			fprintf(stderr, "Failed at address: 0x%x\n", addr);
			break;
		}
		else
		{
			if (addr < mem_begin || addr > mem_end) /* extra check for debug */
			{
				fprintf(stderr, "Invalid address: 0x%x\n", addr);
				continue;
			}

			addr = addr & ~(word_size - 1);  /* Align to word size */
			if (addr % word_size != 0)  /* more debug checks */
			{
				printf("Warning: Unaligned address 0x%x, skipping...\n", addr);
				continue;
			}

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
				printf("Found word!\nFigure out what to do.\n");
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
