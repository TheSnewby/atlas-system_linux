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
	int find_len = strlen(find), replace_len = strlen(replace), word_size = sizeof(long), i;
	int ptrace_rtn = 0, data_size = mem_end - mem_begin;
	int word_count = (replace_len == 0) ? 1 :(word_size + replace_len - 1) / word_size;
	char data[data_size], *data_ptr = data, *find_ptr, command[64];
	char word_buf[word_size * word_size];
	long word, addr, target_ptr;

	// printf("in c\n(%08lx, %08lx)\n", mem_begin, mem_end); /* debug */
	// printf("find: %s\nreplace: %s\n", find, replace);  /* debug */
	// sprintf(command, "cat /proc/%d/maps | grep heap", pid); /* debug */
	// system(command);  /* debug - prints /proc/.../maps in checker */

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
		memcpy(data_ptr, &word, word_size);  /* convert word to string */
		data_ptr += word_size;
	}

	find_ptr = _memmem(data, data_size, find, find_len);  /* find offset */
	target_ptr = mem_begin + find_ptr - data;  /* mem_begin + offset */

	if (find_ptr)  /* if found */
	{
		for (i = 0; i < word_count; i++)  /* populate word_buf */
		{
			word = ptrace(PTRACE_PEEKDATA, pid, target_ptr, NULL);  /* read data at addr */
			if (word == -1)
			{
				fprintf(stderr, "Error in ptrace peek at target_ptr: ");
				perror(NULL);
				fprintf(stderr, "Failed at address: 0x%lx\n", addr);
				return (-1);
			}
			memcpy(&word_buf + (i * word_size), &word, word_size);  /* place long into an char array */
		}

		if (replace_len == 0)
			memset(word_buf, '\0', find_len);
		else if (find_len >= replace_len)  /* fill replacement buffer */
		{
			memcpy(word_buf, replace, replace_len);
			if (find_len > replace_len)
				memset(word_buf + replace_len, '\0', find_len - replace_len);
		}
		else
		{
			fprintf(stderr, "Replace cannot be larger than find.\n");
			return (-1);
		}

		for (i = 0; i < word_count; i++)  /* POKE back the words */
		{
			memcpy(&word, word_buf + (i * word_size), word_size);
			ptrace_rtn = ptrace(PTRACE_POKEDATA, pid, target_ptr, word);
			if (ptrace_rtn == -1)
			{
				fprintf(stderr, "Error in ptrace pokedata: ");
				perror(NULL);
				return (-1);
			}
		}
		// printf("replace succeeded\n");
	}
	else
		fprintf(stderr, "find_ptr not found.\n");

	ptrace_rtn = ptrace(PTRACE_DETACH, pid, 0, 0);
		if (ptrace_rtn == 0)
			// printf("Detached successfully.\n");
	return (0);
}
