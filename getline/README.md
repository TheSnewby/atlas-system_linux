# atlas-system_linux

Learning Objectives
At the end of this project, you are expected to be able to explain to anyone, without the help of Google:

General
What are static variables?
How and when to use them?
Requirements
General
Allowed editors: vi, vim, emacs
All your files will be compiled on Ubuntu 20.04 LTS
Your C programs and functions will be compiled with gcc 9.4.0 using the flags -Wall -Werror -Wextra and -pedantic
All your files should end with a new line
A README.md file, at the root of the folder of the project is mandatory
Your code should use the Betty style. It will be checked using betty-style.pl and betty-doc.pl
All your header files should be include guarded
You are allowed to have up to 6 functions in _getline.c
Allowed Functions and System Calls
read
write
malloc
realloc
free
strcpy
strncpy
strcat
strdup
memset
memcpy

0. Racing cars
Write a function that keeps track of the number of laps made by several cars in a race.

Prototype : void race_state(int *id, size_t size)
id is an array of int representing the “identifier” of each car.
size is the size of this array
Each car identifier is unique
If an identifier is unknown:
Create a new car with the number of laps = 0
Print Car X joined the race followed by a new line (where X is the identifier)
Each time the function is called:
The number of laps of each cars listed in id must be incremented by 1
Print the state of the race:
Header: Race state: followed by a new line
For each car sorted by the identifier: Car X [Y laps] (where X is the identifier and Y the number of laps already done)
If your function is called with size = 0,you must free all allocated memory.

1. _getline
mandatory
Write a function that reads an entire line from a file descriptor.

Prototype : char *_getline(const int fd)
Where fd is the file descriptor to read from
If there are no more lines to return, or if there is an error, the function should return NULL
The function returns a null-terminated string that does not include the newline character
Your header file _getline.h should define a macro called READ_SIZE.
This macro defines the number of bytes you will read each time you will call read: read(fd, buffer, READ_SIZE)
You are not allowed to read more or less than READ_SIZE bytes at once from fd
You are free to pick the value that you want for READ_SIZE
You can assume that fd will always be the same

2. _getline: multi-fd
mandatory
Handle multiple file descriptors.

when called with -1 you should free everything and reset all your static variables