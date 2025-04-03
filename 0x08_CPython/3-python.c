#include <Python.h>
#include <listobject.h>
#include <object.h>
#include <stdio.h>
#include <bytesobject.h>

void print_python_bytes(PyObject *p);
void print_python_float(PyObject *p);

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
	{
		printf("Element %zd: %s\n", i, list->ob_item[i]->ob_type->tp_name);
		if (strcmp(list->ob_item[i]->ob_type->tp_name, "bytes") == 0)
			print_python_bytes(list->ob_item[i]);
		else if (PyFloat_Check(list->ob_item[i]))  /* showcasing two different methods to compare type*/
			print_python_float(list->ob_item[i]);
	}
}

/**
 * print_python_bytes - prints basic information of a Python list up to 10 bytes
 * @p: PyObject
 */
void print_python_bytes(PyObject *p)
{
	Py_ssize_t length, print_len, i;
	PyBytesObject *byte_obj = NULL;

	printf("[.] bytes object info\n");
	if (!p || !PyBytes_Check(p))
	{
		fprintf(stdout, "  [ERROR] Invalid Bytes Object\n");
		return;
	}

	byte_obj = (PyBytesObject *)p;
	length = PyBytes_Size(p);

	if (length < 0)
	{
		fprintf(stdout, "  [ERROR] Invalid Bytes Object\n");
		return;
	}

	print_len = (length < 10) ? length + 1 : 10;
	printf("  size: %zd\n", length);
	printf("  trying string: %s\n", byte_obj->ob_sval);
	printf("  first %zd bytes:", print_len);
	for (i = 0; i < print_len; i++)
		printf(" %02x", (unsigned char)byte_obj->ob_sval[i]);
	printf("\n");
}

/**
 * print_python_float - prints python float pybojects
 * @p: PyFloatObject to be printed
 */
void print_python_float(PyObject *p)
{
	char *p_str;

	printf("[.] float object info\n");

	if (!p || !PyFloat_Check(p))
	{
		fprintf(stdout, "  [ERROR] Invalid Float Object\n");
		return;
	}
	// printf("AH\n");
	p_str = PyOS_double_to_string(((PyFloatObject *)p)->ob_fval, 'g', 16, 0, NULL);
	// printf("BH\n");

	if (p_str)
	{
		printf("  value: %s", p_str);
		if (!strchr(p_str, '.'))
			printf(".0");
		printf("\n");
		PyMem_Free(p_str);
	}
	else
		fprintf(stderr, " [ERROR] Failed to convert float to string\n");
}
