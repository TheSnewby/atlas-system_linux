#include <listobject.h>
#include <object.h>
#include <Python.h>

/**
 * print_python_list - prints basic information of a python list
 * @p: PyObject that should be a list
 */
void print_python_list(PyObject *p)
{
	Py_ssize_t len, allocated, i;
	char *type = NULL;
	PyListObject *obj;

	if (!PyList_Check(p))
	{
		perror("Not a Python List.");
		return;
	}
	obj = (PyListObject *)p;

	len = PyList_size(p);
	allocated = obj->allocated;

	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %d\n", len);
	printf("[*] Allocated = %d\n", allocated);
	for (i = 0; i < len; i++)
	{
		printf("Element %d: %s\n", i, Py_TYPE(obj->ob_item[i])->tp_name);;
	}



}

// [*] Python list info
// [*] Size of the Python List = 2
// [*] Allocated = 2
// Element 0: str
// Element 1: str