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
	long addr;

	//consider doing something with CAP_SYS_PTRACE

	printf("in c\n(%08lx, %08lx)\n", mem_begin, mem_end); /* debug */
	printf("find: %s\nreplace: %s\n", find, replace);  /* debug */
	sprintf(command, "cat /proc/%d/maps | grep heap", pid); /* debug */
	system(command);  /* debug - prints /proc/.../maps in checker */

	ptrace_rtn = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (ptrace_rtn == -1)
	{
		fprintf(stderr, "Error in ptrace attach: ");
		perror(NULL);
		return (-1);
	}

	waitpid(pid, NULL, 0);  /* ensures process is fully stopped */
	int counter = 0;
	for (addr = mem_begin; addr <= mem_end; addr += word_size)
	{
		printf("addr: 0x%8lx\n", addr);
		sprintf(status_path, "/proc/%d/status", pid); /* status check */
		FILE *status_file = fopen(status_path, "r");  /* debug */
		if (!status_file)
		{
			perror("Process has exited");
			return -1;
		}
		fclose(status_file);  /* debug */

		if (kill(pid, 0) == -1)  /* ensure process is still open */
			perror("Process is no longer running: ");

		word = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);  /* read data at addr */
		if (word == -1)
		{
			fprintf(stderr, "Error in ptrace peek: ");
			perror(NULL);
			fprintf(stderr, "Failed at address: 0x%lx\n", addr);
			break;
		}
		else
		{
			if (addr < mem_begin || addr > mem_end) /* extra check for debug */
			{
				fprintf(stderr, "Invalid address: 0x%lx\n", addr);
				continue;
			}

			// addr = addr & ~(word_size - 1);  /* Align to word size */
			// if (addr % word_size != 0)  /* more debug checks */
			// {
			// 	printf("Warning: Unaligned address 0x%x, skipping...\n", addr);
			// 	continue;
			// }

			memcpy(data, &word, word_size);  /* convert read value to string */
			data[word_size - 1] = '\0';
			printf("comparing find: %s, and peek_data: %s\n", find, data);  /* debug */

			if (strcmp(find, data) == 0)  /* look for target word */
			{
				printf("find found!\n");  /* debug */
				memcpy(&word, replace, word_size);  /* store value of replace */

				ptrace_rtn = ptrace(PTRACE_POKEDATA, pid, addr, word);
				if (ptrace_rtn == -1)
				{
					fprintf(stderr, "Error in ptrace poke: ");
					perror(NULL);
				}
				break;
			}
			else if (strstr(data, find) != NULL)  /* find in word */
			{
				printf("Found word!\nFigure out what to do.\n");
				break;
			}
			/* have a case for *find being found in across multiple words? */
		}
		printf("counter: %d\n", counter++);
	}

	ptrace_rtn = ptrace(PTRACE_DETACH, pid, 0, 0);  /* consider adding this to all errors */
	if (ptrace_rtn == 0)
		printf("Detached successfully.\n");
	return (0);
}

int main(void)
{
	char *mem_begin_str= "55d11d33d000";
	char *mem_end_str = "55d11d35e000";
	char *begin_ptr;
	char *end_ptr;
	long mem_begin = strtol(mem_begin_str, &begin_ptr, 16);
	long mem_end = strtol(mem_end_str, &end_ptr, 16);

	heap_rw(58535, mem_begin, mem_end, "hi", "ho");
	return 0;
}
