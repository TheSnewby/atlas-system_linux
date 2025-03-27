#include "hnm.h"

/**
 * get_symbol_type_64 - Determine symbol type for 64-bit symbol
 * @sym: 64-bit symbol entry
 * 
 * Return: Symbol type character
 */
char get_symbol_type_64(Elf64_Sym *sym)
{
    char c;

    /* Global Symbols */
    if (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL) {
        switch (ELF64_ST_TYPE(sym->st_info)) {
            case STT_FUNC: c = 'T'; break;
            case STT_OBJECT: c = 'D'; break;
            case STT_SECTION: c = 'A'; break;
            default: c = 'U'; break;
        }
    }
    /* Local Symbols */
    else {
        switch (ELF64_ST_TYPE(sym->st_info)) {
            case STT_FUNC: c = 't'; break;
            case STT_OBJECT: c = 'd'; break;
            case STT_SECTION: c = 'a'; break;
            default: c = 'u'; break;
        }
    }

    /* Special Case */
    if (sym->st_shndx == SHN_UNDEF)
        c = 'U';

    return c;
}

/**
 * get_symbol_type_32 - Determine symbol type for 32-bit symbol
 * @sym: 32-bit symbol entry
 * 
 * Return: Symbol type character
 */
char get_symbol_type_32(Elf32_Sym *sym)
{
    char c;

    /* Global Symbols */
    if (ELF32_ST_BIND(sym->st_info) == STB_GLOBAL) {
        switch (ELF32_ST_TYPE(sym->st_info)) {
            case STT_FUNC: c = 'T'; break;
            case STT_OBJECT: c = 'B'; break; /* formerly 'D' */
            case STT_SECTION: c = 'W'; break; /* formerly 'A' */
            default: c = 'U'; break;
        }
    }
    /* Local Symbols */
    else {
        switch (ELF32_ST_TYPE(sym->st_info)) {
            case STT_FUNC: c = 'W'; break;  /* formerly 't' */
            case STT_OBJECT: c = 'd'; break;
            case STT_SECTION: c = 'a'; break;
            default: c = 'u'; break;
        }
    }

    /* Special Case */
    if (sym->st_shndx == SHN_UNDEF)
        c = 'U';

    return c;
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
                char symbol_type = get_symbol_type_64(&symbols_64[i]);

                /* Print Symbol */
                if (symbols_64[i].st_value || symbol_type == 'U')
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
                char symbol_type = get_symbol_type_32(&symbols_32[i]);

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
