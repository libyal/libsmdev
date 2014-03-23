/*
 * Python bindings module for libsmdev (pysmdev)
 *
 * Copyright (c) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pysmdev.h"
#include "pysmdev_error.h"
#include "pysmdev_handle.h"
#include "pysmdev_libcerror.h"
#include "pysmdev_libcstring.h"
#include "pysmdev_libsmdev.h"
#include "pysmdev_python.h"
#include "pysmdev_unused.h"

/* The pysmdev module methods
 */
PyMethodDef pysmdev_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pysmdev_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "open",
	  (PyCFunction) pysmdev_handle_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a handle." },

	/* Sentinel */
	{ NULL,
	  NULL,
	  0,
	  NULL}
};

/* Retrieves the pysmdev/libsmdev version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmdev_get_version(
           PyObject *self PYSMDEV_ATTRIBUTE_UNUSED,
           PyObject *arguments PYSMDEV_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYSMDEV_UNREFERENCED_PARAMETER( self )
	PYSMDEV_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libsmdev_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = libcstring_narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

/* Declarations for DLL import/export
 */
#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

/* Initializes the pysmdev module
 */
PyMODINIT_FUNC initpysmdev(
                void )
{
	PyObject *module                 = NULL;
	PyTypeObject *handle_type_object = NULL;
	PyGILState_STATE gil_state       = 0;

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
	module = Py_InitModule3(
	          "pysmdev",
	          pysmdev_module_methods,
	          "Python libsmdev module (pysmdev)." );

	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the handle type object
	 */
	pysmdev_handle_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pysmdev_handle_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pysmdev_handle_type_object );

	handle_type_object = &pysmdev_handle_type_object;

	PyModule_AddObject(
	 module,
	 "handle",
	 (PyObject *) handle_type_object );

on_error:
	PyGILState_Release(
	 gil_state );
}

