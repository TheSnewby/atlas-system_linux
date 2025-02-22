/**
 * main - ELF file header reader
 * @argc: number of arguments
 * @argv: array of strings
 *
 * Return: 0 if success, errno otherwise
 */
int main(int argc, char **argv)
{
	int i;

	if (argc < 2)
		return (0);

	for (i = 0; i < argc; i++)
		printf("%s\n", argv[i]);

	return (0);
}
