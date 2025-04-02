#include <Python.h>
#include <listobject.h>
#include <object.h>
#include <stdio.h>
#include <bytesobject.h>

/**
 * print_python_list - print basic information of a Python list
 * @p: PyObject
 */
void print_python_list(PyObject *p)
{
	Py_ssize_t len, allocated, i;
	PyListObject *list;

	if (!PyList_Check(p))
	{
		perror("Not a Python List.");
		return;
	}
	list = (PyListObject *)p;

	len = ((PyVarObject *)p)->ob_size;
	allocated = list->allocated;

	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %zd\n", len);
	printf("[*] Allocated = %zd\n", allocated);
	for (i = 0; i < len; i++)
		printf("Element %zd: %s\n", i, list->ob_item[i]->ob_type->tp_name);
}

/**
 * print_python_bytes - prints basic information of a Python list up to 10 bytes
 * @p: PyObject
 */
void print_python_bytes(PyObject *p)
{
	char *buffer;
	Py_ssize_t length, print_len, i;

	printf("[.] bytes object info\n");
	if (!PyBytes_Check(p))
	{
		fprintf(stderr, "  [ERROR] Invalid Bytes Object\n");
		return;
	}

	PyBytes_AsStringAndSize(p, &buffer, &length);

	if (length < 0)
		return;

	print_len = (length < 10) ? length + 1 : 10;
	printf("  size: %zd\n", length);
	printf("  trying string: %s\n", buffer);
	printf("  first %zd bytes:", print_len);
	for (i = 0; i < print_len; i++)
		printf(" %02x", (unsigned char)buffer[i]);
	printf("\n");
}
