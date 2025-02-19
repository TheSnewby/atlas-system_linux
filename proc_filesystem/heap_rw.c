#include <stdio.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>


/**
 * _memmem - finds the pointer to a substring in a string based on GNU's memmem
 * @haystack: string
 * @haystack_len: length of haystack
 * @needle: substring target
 * @needle_len: needle length
 *
 * Return: pointer to memory location if found, NULL if not
 */
char *_memmem(const char *haystack, size_t haystack_len, const char *needle, size_t needle_len) {
    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        if (memcmp(haystack + i, needle, needle_len) == 0)
            return (char *)(haystack + i);
    }
    return NULL;
}


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
	int find_len = strlen(find), replace_len = strlen(replace), word_size = sizeof(long);
	int ptrace_rtn = 0, data_size = mem_end - mem_begin;
	char data[data_size], *data_ptr = data, *find_ptr, command[64], word_buf[word_size];
	long word, addr, target_ptr;

	printf("in c\n(%08lx, %08lx)\n", mem_begin, mem_end); /* debug */
	printf("find: %s\nreplace: %s\n", find, replace);  /* debug */
	sprintf(command, "cat /proc/%d/maps | grep heap", pid); /* debug */
	system(command);  /* debug - prints /proc/.../maps in checker */

	ptrace_rtn = ptrace(PTRACE_ATTACH, pid, 0, 0);  /* attach to process */
	if (ptrace_rtn == -1)
	{
		fprintf(stderr, "Error in ptrace attach: ");
		perror(NULL);
		return (-1);
	}

	waitpid(pid, NULL, 0);  /* ensure the pid actually stopped */
	// int counter = 0;

	for (addr = mem_begin; addr <= mem_end - word_size; addr += word_size)
	{
		word = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);  /* read data at addr */
		if (word == -1)
		{
			fprintf(stderr, "Error in ptrace peek: ");
			perror(NULL);
			fprintf(stderr, "Failed at address: 0x%lx\n", addr);
			return (-1);
		}
		memcpy(data_ptr, &word, word_size);  /* convert read value to string */
		data_ptr += word_size;
	}

	find_ptr = _memmem(data, data_size, find, find_len);  /* find offset */
	target_ptr = mem_begin + find_ptr - data;  /* mem_begin + offset */

	if (find_ptr)
	{
		word = ptrace(PTRACE_PEEKDATA, pid, target_ptr, NULL);  /* read data at addr */
		if (word == -1)
		{
			fprintf(stderr, "Error in ptrace peek at target_ptr: ");
			perror(NULL);
			fprintf(stderr, "Failed at address: 0x%lx\n", addr);
			return (-1);
		}

		memcpy(&word_buf, &word, word_size);  /* place long into an char array */

		/* setup replacement buffer */
		if ((find_len == replace_len) && (replace_len <= word_size))
		{
			memcpy(word_buf, replace, replace_len);
		}
		else if ((find_len > replace_len) && (replace_len <= word_size))
		{
			memcpy(word_buf, replace, replace_len);
			memset(word_buf + replace_len, '\0', replace_len - find_len);
		}
		else
			fprintf(stderr, "Replace cannot be larger than find.\n");

		word = *(long *)word_buf;  /* convert char array back into a long */

		if (replace_len <= word_size)
		{
			ptrace_rtn = ptrace(PTRACE_POKEDATA, pid, target_ptr, word);
			if (ptrace_rtn == -1)
			{
				fprintf(stderr, "Error in ptrace pokedata: ");
				perror(NULL);
				return (-1);
			}
			printf("replace succeeded\n");
		}
		else
			fprintf(stderr, "Ruh Roh, make a plan for handling larger than 1 word_size\n");
	}
	else
		fprintf(stderr, "find_ptr not found.\n");

	ptrace_rtn = ptrace(PTRACE_DETACH, pid, 0, 0);
		if (ptrace_rtn == 0)
			printf("Detached successfully.\n");
	return (0);
}

// int main(void)
// {
// 	char *mem_begin_str= "55b7a4f34000";
// 	char *mem_end_str = "55b7a4f55000";
// 	char *begin_ptr;
// 	char *end_ptr;
// 	long mem_begin = strtol(mem_begin_str, &begin_ptr, 16);
// 	long mem_end = strtol(mem_end_str, &end_ptr, 16);

// 	heap_rw(376343, mem_begin, mem_end, "hi", "ho");
// 	return 0;
// }

// int heap_rw(int pid, long mem_begin, long mem_end, char *find, char *replace)
// {
// 	int ptrace_rtn = 0;
// 	size_t word_size = sizeof(long);
// 	char data[word_size], command[64], status_path[64];;
// 	long word;
// 	long addr;

// 	//consider doing something with CAP_SYS_PTRACE

// 	printf("in c\n(%08lx, %08lx)\n", mem_begin, mem_end); /* debug */
// 	printf("find: %s\nreplace: %s\n", find, replace);  /* debug */
// 	sprintf(command, "cat /proc/%d/maps | grep heap", pid); /* debug */
// 	system(command);  /* debug - prints /proc/.../maps in checker */

// 	ptrace_rtn = ptrace(PTRACE_ATTACH, pid, 0, 0);
// 	if (ptrace_rtn == -1)
// 	{
// 		fprintf(stderr, "Error in ptrace attach: ");
// 		perror(NULL);
// 		return (-1);
// 	}

// 	waitpid(pid, NULL, 0);  /* ensures process is fully stopped */
// 	int counter = 0;

// 	for (addr = mem_begin; addr < mem_end; addr += word_size)
// 	{
// 		// printf("addr: 0x%8lx\n", addr);
// 		sprintf(status_path, "/proc/%d/status", pid); /* status check */
// 		FILE *status_file = fopen(status_path, "r");  /* debug */
// 		if (!status_file)
// 		{
// 			perror("Process has exited");
// 			return -1;
// 		}
// 		fclose(status_file);  /* debug */

// 		if (kill(pid, 0) == -1)  /* ensure process is still open */
// 			perror("Process is no longer running: ");

// 		word = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);  /* read data at addr */
// 		if (word == -1)
// 		{
// 			fprintf(stderr, "Error in ptrace peek: ");
// 			perror(NULL);
// 			fprintf(stderr, "Failed at address: 0x%lx\n", addr);
// 			break;
// 		}
// 		else
// 		{
// 			if (addr < mem_begin || addr > mem_end) /* extra check for debug */
// 			{
// 				fprintf(stderr, "Invalid address: 0x%lx\n", addr);
// 				continue;
// 			}

// 			addr = addr & ~(word_size - 1);  /* Align to word size */
// 			if (addr % word_size != 0)  /* more debug checks */
// 			{
// 				printf("Warning: Unaligned address 0x%lx, skipping...\n", addr);
// 				continue;
// 			}

// 			memcpy(data, &word, word_size);  /* convert read value to string */
// 			data[word_size - 1] = '\0';
// 			printf("at addr: 0x%8lx: peek_data: %s\n", addr, data);  /* debug */

// 			if (strcmp(find, data) == 0)  /* look for target word */
// 			{
// 				printf("find found!\n");  /* debug */
// 				memcpy(&word, replace, word_size);  /* store value of replace */

// 				ptrace_rtn = ptrace(PTRACE_POKEDATA, pid, addr, word);
// 				if (ptrace_rtn == -1)
// 				{
// 					fprintf(stderr, "Error in ptrace poke: ");
// 					perror(NULL);
// 				}
// 				break;
// 			}
// 			else if (strstr(data, find) != NULL)  /* find in word */
// 			{
// 				printf("Found word!\nFigure out what to do.\n");
// 				break;
// 			}
// 			/* have a case for *find being found in across multiple words? */
// 		}
// 		// printf("counter: %d\n", counter++);
// 		}

// 	ptrace_rtn = ptrace(PTRACE_DETACH, pid, 0, 0);  /* consider adding this to all errors */
// 	if (ptrace_rtn == 0)
// 		printf("Detached successfully.\n");
// 	return (0);
// }