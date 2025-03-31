#include <Python.h>
#include <listobject.h>
#include <object.h>
#include <stdio.h>

/**
 * print_python_list - prints basic information of a python list
 * @p: PyObject that should be a list
 */
void print_python_list(PyObject *p)
{
	Py_ssize_t len, allocated, i;
	char *type = NULL;
	PyListObject *list;

	if (!PyList_Check(p))
	{
		perror("Not a Python List.");
		return;
	}
	list = (PyListObject *)p;

	len = PyList_size(p);
	allocated = list->allocated;

	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %zd\n", len);
	printf("[*] Allocated = %zd\n", allocated);
	for (i = 0; i < len; i++)
	{
		printf("Element %zd: %s\n", i, list->ob_item[i])->ob_type->tp_name;
	}
}

// [*] Python list info
// [*] Size of the Python List = 2
// [*] Allocated = 2
// Element 0: str
// Element 1: str