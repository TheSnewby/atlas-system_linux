#include <Python.h>
#include <object.h>
#include <stdio.h>
#include <longintrepr.h>
#include <limits.h>

/**
 * print_python_int - prints a Python Int
 * @p: PyObject of a Python Int
 */
void print_python_int(PyObject *p)
{
    if (!PyLong_Check(p))
    {
        printf("Invalid Int Object\n");
        return;
    }

    PyLongObject *p_long = (PyLongObject *)p;
    Py_ssize_t size = ((PyVarObject *)p)->ob_size, abs_size;
    Py_ssize_t i;
    unsigned long int sum = 0;
	char *sign;

	if (size < 0)
		sign = "-";
	else
		sign = "";

	abs_size = (size >= 0) ? size : -size;

	// printf("[DEBUG]size:       %lu\n", abs_size);
	// Py_ssize_t max_digits = ((sizeof(unsigned long int) * 8) + PyLong_SHIFT - 1) / 8;
	// printf("[DEBUG]comparison: %lu\n", max_digits);

	// printf("[DEBUG]ulong_max:  %lu\n", ULONG_MAX);

    /* Check if the number is too large */
    if ((unsigned long int)abs_size > (sizeof(unsigned long int) * CHAR_BIT + PyLong_SHIFT - 1) / PyLong_SHIFT)
    {
        printf("C unsigned long int overflow\n");
        return;
    }

	digit *py_integer = p_long->ob_digit;

    /* Convert Python int to unsigned long int */
    for (i = 0; i < abs_size; i++)
    {
        sum += (unsigned long int)py_integer[i] << (PyLong_SHIFT * i);
    }

    /* Print the integer */
    printf("%s%lu\n", sign, sum);
}
