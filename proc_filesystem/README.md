#Learning Objectives
At the end of this project, you are expected to be able to explain to anyone, without the help of Google:

General
What is the /proc filesystem
What is the /proc/maps file
What is the /proc/mem file
What is the link between those two files
How does the virtual memory map into the RAM
Requirements
Python Scripts
Allowed editors: vi, vim, emacs
All your files will be interpreted/compiled on Ubuntu 20.04 LTS using python3 (version 3.9)
All your files should end with a new line
The first line of all your files should be exactly #!/usr/bin/python3
A README.md file, at the root of the folder of the project, is mandatory
Your code should use the PEP 8 style
All your files must be executable
The length of your files will be tested using wc
Python Test Cases
Allowed editors: vi, vim, emacs
All your files should end with a new line
All your modules should have a documentation (python3 -c 'print(__import__("my_module").__doc__)')
All your classes should have a documentation (python3 -c 'print(__import__("my_module").MyClass.__doc__)')
All your functions (inside and outside a class) should have a documentation (python3 -c 'print(__import__("my_module").my_function.__doc__)' and python3 -c 'print(__import__("my_module").MyClass.my_function.__doc__)')
We strongly encourage you to work together on test cases, so that you don’t miss any edge case
Tasks
0. Hack the VM
mandatory
Write a script that finds a string in the heap of a running process, and replaces it.

Usage: read_write_heap.py pid search_string replace_string
where pid is the pid of the running process
and strings are ASCII
The script should look only in the heap of the process
Output: you can print whatever you think is interesting
On usage error, print an error message on stdout and exit with status code 1