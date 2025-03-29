#include "hnm.h"

/**
 * get_symbol_type_64 - Determine symbol type for 64-bit symbol
 * @sym: 64-bit symbol entry
 * @sections: 64-bit section header
 * @shstrtab: buffer containing section header string table
 * 
 * Return: Symbol type character
 */
char get_symbol_type_64(Elf64_Sym *sym, Elf64_Shdr *sections, char *shstrtab)
{
	unsigned char binding = ELF64_ST_BIND(sym->st_info); /* local, global, weak */
	unsigned char type = ELF64_ST_TYPE(sym->st_info); /* function, object, file */
	Elf64_Shdr *section = NULL;  /* individual section */
	char *name = NULL;  /* section name*/

	if (type == STT_FILE)
		return (0);

	if ((type == STT_NOTYPE) || (sym->st_shndx == SHN_UNDEF))
		return ('U');

	if (binding == STB_WEAK)
	{
		if (sym->st_shndx == SHN_UNDEF)
			return ((type == STT_OBJECT) ? 'v' : 'w');
		else
			return ((type == STT_OBJECT) ? 'V' : 'W');
	}

	if (sym->st_shndx == SHN_ABS)
		return ((binding == STB_GLOBAL) ? 'A' : 'a');

	if (sym->st_shndx == SHN_COMMON)
		return ((binding == STB_GLOBAL) ? 'C' : 'c');

	if (type == STT_FUNC)
		return ((binding == STB_GLOBAL) ? 'T' : 't');

	section = &sections[sym->st_shndx];

	if (section->sh_type == SHT_NOBITS)
		return ((binding == STB_GLOBAL) ? 'B' : 'b');

	if (!shstrtab)
		return ('?');

	name = shstrtab + section->sh_name;

	if ((strcmp(name, ".init_array") == 0) ||
	(strcmp(name, ".fini_array") == 0) ||
	(strcmp(name, ".text") == 0) ||
	(section->sh_flags & SHF_EXECINSTR))
		return ((binding == STB_GLOBAL) ? 'T' : 't');

	if ((strcmp(name, ".data") == 0) || ((section->sh_flags & SHF_ALLOC) && (section->sh_flags & SHF_WRITE)))
		return ((binding == STB_GLOBAL) ? 'D' : 'd');

	if (strcmp(name, ".bss") == 0)
		return ((binding == STB_GLOBAL) ? 'B' : 'b');

	if ((strcmp(name, ".rodata") == 0) || (section->sh_flags & SHF_ALLOC))
		return ((binding == STB_GLOBAL) ? 'R' : 'r');

	return ('?');
	// return (0);
}


/**
 * get_symbol_type_32 - Determine symbol type for 32-bit symbol
 * @sym: 32-bit symbol entry
 * @sections: 64-bit section header
 * @shstrtab: buffer containing section header string table
 * 
 * Return: Symbol type character
 */
char get_symbol_type_32(Elf32_Sym *sym, Elf32_Shdr *sections, char *shstrtab)
{
	unsigned char binding = ELF32_ST_BIND(sym->st_info); /* local, global, weak */
	unsigned char type = ELF32_ST_TYPE(sym->st_info); /* function, object, file */
	Elf32_Shdr *section = NULL;  /* individual section */
	char *name = NULL;  /* section name*/

	if (type == STT_FILE)
		return (0);

	if (binding == STB_WEAK)
	{
		if (sym->st_shndx == SHN_UNDEF)
			return ((type == STT_OBJECT) ? 'v' : 'w');
		else
			return ((type == STT_OBJECT) ? 'V' : 'W');
	}

	if (sym->st_shndx == SHN_COMMON)
		return ((binding == STB_GLOBAL) ? 'C' : 'c');

	if (type == STT_FUNC)
		return ((binding == STB_GLOBAL) ? 'T' : 't');

	if (sym->st_shndx == SHN_UNDEF)
		return ('U');

	if (sym->st_shndx == SHN_ABS)
		return ((binding == STB_GLOBAL) ? 'A' : 'a');

	section = &sections[sym->st_shndx];

	if (section->sh_type == SHT_NOBITS)
		return ((binding == STB_GLOBAL) ? 'B' : 'b');

	if (!shstrtab)
		return ('?');

	name = shstrtab + section->sh_name;

	if ((strcmp(name, ".init_array") == 0) ||
	(strcmp(name, ".fini_array") == 0) ||
	(strcmp(name, ".text") == 0) ||
	(section->sh_flags & SHF_EXECINSTR))
		return ((binding == STB_GLOBAL) ? 'T' : 't');

	if ((strcmp(name, ".data") == 0) || ((section->sh_flags & SHF_ALLOC) && (section->sh_flags & SHF_WRITE)))
		return ((binding == STB_LOCAL) ? 'D' : 'd');

	if (strcmp(name, ".bss") == 0)
		return ((binding == STB_LOCAL) ? 'B' : 'b');

	if ((strcmp(name, ".rodata") == 0) || (section->sh_flags & SHF_ALLOC))
		return ((binding == STB_LOCAL) ? 'R' : 'r');

	return ('?');
	// return (0);
}

int parse_symbol_table(const char *file_path)
{
    FILE *fptr;
    unsigned char elf_header[64] = {0};
    int is_64bit, is_big_endian, num_symbols, i;
    char *shstrtab = NULL, *strtab = NULL;

    fptr = fopen(file_path, "rb");
    if (!fptr)
	{
        perror("Error Opening File");
        return -1;
    }

    if (fread(elf_header, 1, 64, fptr) != 64)
	{
        perror("Error Reading ELF Header");
        fclose(fptr);
        return -1;
    }

    /* Determine Architecture And Endianness */
    is_big_endian = (elf_header[5] == 2);
    is_64bit = (elf_header[4] == 2);

    if (is_64bit)
	{
        Elf64_Ehdr header;
        Elf64_Shdr *section_headers, *symtab_shdr = NULL, *strtab_shdr = NULL, *str_section;

        memcpy(&header, elf_header, sizeof(header));

        /* Swap Values If Needed */
        header.e_shoff = swap_uint64(header.e_shoff, is_big_endian);
        header.e_shnum = swap_uint16(header.e_shnum, is_big_endian);
        header.e_shstrndx = swap_uint16(header.e_shstrndx, is_big_endian);

        /* Allocate Section Headers */
        section_headers = malloc(header.e_shnum * sizeof(Elf64_Shdr));
        if (!section_headers)
		{
            perror("Memory Allocation Error");
            fclose(fptr);
            return -1;
        }

        /* Read Section Headers */
        fseek(fptr, header.e_shoff, SEEK_SET);
        if (fread(section_headers, sizeof(Elf64_Shdr), header.e_shnum, fptr) 
            != header.e_shnum)
		{
            perror("Error Reading Section Headers");
            free(section_headers);
            fclose(fptr);
            return -1;
        }

        /* Read Section Header String Table */
        str_section = &section_headers[header.e_shstrndx];
        str_section->sh_offset = swap_uint64(str_section->sh_offset, is_big_endian);
        str_section->sh_size = swap_uint64(str_section->sh_size, is_big_endian);

        shstrtab = malloc(str_section->sh_size);
        if (!shstrtab)
		{
            perror("Memory Allocation Error For String Table");
            free(section_headers);
            fclose(fptr);
            return -1;
        }

        fseek(fptr, str_section->sh_offset, SEEK_SET);
        if (fread(shstrtab, 1, str_section->sh_size, fptr) 
            != str_section->sh_size)
		{
            perror("Error Reading String Table");
            free(section_headers);
            free(shstrtab);
            fclose(fptr);
            return -1;
        }

        /* Find Symbol Table And String Table Sections */
        for (i = 0; i < header.e_shnum; i++)
		{
            section_headers[i].sh_name = swap_uint32(section_headers[i].sh_name, is_big_endian);
            section_headers[i].sh_type = swap_uint32(section_headers[i].sh_type, is_big_endian);
            section_headers[i].sh_offset = swap_uint64(section_headers[i].sh_offset, is_big_endian);
            section_headers[i].sh_size = swap_uint64(section_headers[i].sh_size, is_big_endian);

            /* Find Symbol Table */
            if (section_headers[i].sh_type == SHT_SYMTAB)
                symtab_shdr = &section_headers[i];
            /* Find Corresponding String Table */
            else if (section_headers[i].sh_type == SHT_STRTAB)
			{
                if (i != header.e_shstrndx)
                    strtab_shdr = &section_headers[i];
            }
        }

        /* Process Symbols */
        if (symtab_shdr && strtab_shdr)
		{
            /* Read String Table */
            strtab = malloc(strtab_shdr->sh_size);
            if (!strtab)
			{
                perror("Memory Allocation Error For String Table");
                free(section_headers);
                free(shstrtab);
                fclose(fptr);
                return -1;
            }

            fseek(fptr, strtab_shdr->sh_offset, SEEK_SET);
            if (fread(strtab, 1, strtab_shdr->sh_size, fptr) 
                != strtab_shdr->sh_size)
			{
                perror("Error Reading String Table");
                free(section_headers);
                free(shstrtab);
                free(strtab);
                fclose(fptr);
                return -1;
            }

            /* Process Symbol Table */
            num_symbols = symtab_shdr->sh_size / sizeof(Elf64_Sym);
            Elf64_Sym *symbols_64 = malloc(symtab_shdr->sh_size);
            if (!symbols_64)
			{
                perror("Memory Allocation Error For Symbols");
                free(section_headers);
                free(shstrtab);
                free(strtab);
                fclose(fptr);
                return -1;
            }

            fseek(fptr, symtab_shdr->sh_offset, SEEK_SET);
            if (fread(symbols_64, sizeof(Elf64_Sym), num_symbols, fptr) 
                != (size_t)num_symbols)
			{
                perror("Error Reading Symbols");
                free(section_headers);
                free(shstrtab);
                free(strtab);
                free(symbols_64);
                fclose(fptr);
                return -1;
            }

            /* Print Symbols */
            for (i = 0; i < num_symbols; i++)
			{
                /* Swap Symbol Values If Needed */
                symbols_64[i].st_value = swap_uint64(symbols_64[i].st_value, is_big_endian);
                symbols_64[i].st_shndx = swap_uint16(symbols_64[i].st_shndx, is_big_endian);

                /* Get Symbol Name */
                const char *name = &strtab[symbols_64[i].st_name];

                /* Get Symbol Type */
                char symbol_type = get_symbol_type_64(&symbols_64[i], section_headers, shstrtab);

                /* Print Symbol */
                if ((symbol_type != 'a') && (strcmp(name, "") != 0) && (symbols_64[i].st_value || symbol_type == 'U'))
				{
                    if (symbol_type == 'U')
                        printf("                 %c %s\n", symbol_type, name);
                    else
                        printf("%016lx %c %s\n", symbols_64[i].st_value, symbol_type, name);
                }
            }
            free(symbols_64);
            free(strtab);
        }
        free(section_headers);
        free(shstrtab);
        fclose(fptr);
    }
    else
	{
        Elf32_Ehdr header;
        Elf32_Shdr *section_headers, *symtab_shdr = NULL, *strtab_shdr = NULL, *str_section;

        memcpy(&header, elf_header, sizeof(header));

        /* Swap Values If Needed */
        header.e_shoff = swap_uint32(header.e_shoff, is_big_endian);
        header.e_shnum = swap_uint16(header.e_shnum, is_big_endian);
        header.e_shstrndx = swap_uint16(header.e_shstrndx, is_big_endian);

        /* Allocate Section Headers */
        section_headers = malloc(header.e_shnum * sizeof(Elf32_Shdr));
        if (!section_headers)
		{
            perror("Memory Allocation Error");
            fclose(fptr);
            return -1;
        }

        /* Read Section Headers */
        fseek(fptr, header.e_shoff, SEEK_SET);
        if (fread(section_headers, sizeof(Elf32_Shdr), header.e_shnum, fptr) 
            != header.e_shnum)
		{
            perror("Error Reading Section Headers");
            free(section_headers);
            fclose(fptr);
            return -1;
        }

        /* Read Section Header String Table */
        str_section = &section_headers[header.e_shstrndx];
        str_section->sh_offset = swap_uint32(str_section->sh_offset, is_big_endian);
        str_section->sh_size = swap_uint32(str_section->sh_size, is_big_endian);

        shstrtab = malloc(str_section->sh_size);
        if (!shstrtab)
		{
            perror("Memory Allocation Error For String Table");
            free(section_headers);
            fclose(fptr);
            return -1;
        }

        fseek(fptr, str_section->sh_offset, SEEK_SET);
        if (fread(shstrtab, 1, str_section->sh_size, fptr) 
            != str_section->sh_size)
		{
            perror("Error Reading String Table");
            free(section_headers);
            free(shstrtab);
            fclose(fptr);
            return -1;
        }

        /* Find Symbol Table And String Table Sections */
        for (i = 0; i < header.e_shnum; i++)
		{
            section_headers[i].sh_name = swap_uint32(section_headers[i].sh_name, is_big_endian);
            section_headers[i].sh_type = swap_uint32(section_headers[i].sh_type, is_big_endian);
            section_headers[i].sh_offset = swap_uint32(section_headers[i].sh_offset, is_big_endian);
            section_headers[i].sh_size = swap_uint32(section_headers[i].sh_size, is_big_endian);

            /* Find Symbol Table */
            if (section_headers[i].sh_type == SHT_SYMTAB)
                symtab_shdr = &section_headers[i];
            /* Find Corresponding String Table */
            else if (section_headers[i].sh_type == SHT_STRTAB)
			{
                if (i != header.e_shstrndx)
                    strtab_shdr = &section_headers[i];
            }
        }

        /* Process Symbols */
        if (symtab_shdr && strtab_shdr) {
            /* Read String Table */
            strtab = malloc(strtab_shdr->sh_size);
            if (!strtab)
			{
                perror("Memory Allocation Error For String Table");
                free(section_headers);
                free(shstrtab);
                fclose(fptr);
                return -1;
            }

            fseek(fptr, strtab_shdr->sh_offset, SEEK_SET);
            if (fread(strtab, 1, strtab_shdr->sh_size, fptr) 
                != strtab_shdr->sh_size)
			{
                perror("Error Reading String Table");
                free(section_headers);
                free(shstrtab);
                free(strtab);
                fclose(fptr);
                return -1;
            }

            /* Process Symbol Table */
            num_symbols = symtab_shdr->sh_size / sizeof(Elf32_Sym);
            Elf32_Sym *symbols_32 = malloc(symtab_shdr->sh_size);
            if (!symbols_32)
			{
                perror("Memory Allocation Error For Symbols");
                free(section_headers);
                free(shstrtab);
                free(strtab);
                fclose(fptr);
                return -1;
            }

            fseek(fptr, symtab_shdr->sh_offset, SEEK_SET);
            if (fread(symbols_32, sizeof(Elf32_Sym), num_symbols, fptr) 
                != (size_t)num_symbols)
			{
                perror("Error Reading Symbols");
                free(section_headers);
                free(shstrtab);
                free(strtab);
                free(symbols_32);
                fclose(fptr);
                return -1;
            }

            /* Print Symbols */
            for (i = 0; i < num_symbols; i++)
			{
                /* Swap Symbol Values If Needed */
                symbols_32[i].st_value = swap_uint32(symbols_32[i].st_value, is_big_endian);
                symbols_32[i].st_shndx = swap_uint16(symbols_32[i].st_shndx, is_big_endian);

                /* Get Symbol Name */
                const char *name = &strtab[symbols_32[i].st_name];

                /* Get Symbol Type */
                char symbol_type = get_symbol_type_32(&symbols_32[i], section_headers, shstrtab);

                /* Print Symbol */
                if ((symbol_type != 'a') && (strcmp(name, "") != 0) && (symbols_32[i].st_value || symbol_type == 'U')) {
                    if (symbol_type == 'U')
                        printf("         %c %s\n", symbol_type, name);
                    else
                        printf("%08x %c %s\n", symbols_32[i].st_value, symbol_type, name);
                }
            }
            free(symbols_32);
            free(strtab);
        }
        free(section_headers);
        free(shstrtab);
        fclose(fptr);
    }

    return 0;
}

/**
 * main - Entry point for the ELF symbol table reader
 * @argc: Argument count
 * @argv: Argument vector
 * 
 * Return: 0 on success, -1 on failure
 */
int main(int argc, char **argv)
{
    if (argc < 2)
	{
        fprintf(stderr, "Usage: %s <elf_file>\n", argv[0]);
        return -1;
    }

    return parse_symbol_table(argv[1]);
}
