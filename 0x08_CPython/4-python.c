#include <Python.h>
#include <object.h>
#include <stdio.h>

/**
 * print_python_string - prints a Python string to stdout
 * @p: PyObject String
 */
void print_python_string(PyObject *p)
{
	char *string_type = NULL;
	printf("[.] string object info\n");

	if (!PyUnicode_Check(p))
	{
		fprintf(stdout, "  [ERROR] Invalid String Object\n");
		return;
	}

	switch (PyUnicode_KIND(p)) {
		case PyUnicode_1BYTE_KIND:
			string_type = "compact ascii";
			break;
		case PyUnicode_2BYTE_KIND:
			string_type = "compact unicode object";
			break;
		case PyUnicode_4BYTE_KIND:
			string_type = "wide unicode object";
			break;
		default:
			string_type = "unknown";
			break;
	}
	printf("  type: %s\n", string_type);

	const char *str = PyUnicode_AsUTF8(p);
	printf("  length: %ld\n", PyUnicode_GET_LENGTH(p));
	printf("  value: %s\n", str);

	PyMem_Free((void *)str);
}