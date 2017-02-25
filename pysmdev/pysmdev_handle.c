/*
 * Python object wrapper of libsmdev_handle_t
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pysmdev_error.h"
#include "pysmdev_handle.h"
#include "pysmdev_integer.h"
#include "pysmdev_libcerror.h"
#include "pysmdev_libsmdev.h"
#include "pysmdev_metadata.h"
#include "pysmdev_python.h"
#include "pysmdev_unused.h"

PyMethodDef pysmdev_handle_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pysmdev_handle_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the handle to abort the current activity." },

	/* Functions to access the media data */

	{ "open",
	  (PyCFunction) pysmdev_handle_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a handle." },

	{ "close",
	  (PyCFunction) pysmdev_handle_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes file(s)." },

	{ "read_buffer",
	  (PyCFunction) pysmdev_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> String\n"
	  "\n"
	  "Reads a buffer of media data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pysmdev_handle_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> String\n"
	  "\n"
	  "Reads a buffer of media data at a specific offset." },

	{ "write_buffer",
	  (PyCFunction) pysmdev_handle_write_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "write_buffer(buffer) -> None\n"
	  "\n"
	  "Writes a buffer of media data." },

	{ "write_buffer_at_offset",
	  (PyCFunction) pysmdev_handle_write_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "write_buffer_at_offset(buffer, offset) -> None\n"
	  "\n"
	  "Writes a buffer of media data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pysmdev_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the media data." },

	{ "get_offset",
	  (PyCFunction) pysmdev_handle_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the media data." },

	/* Some Pythonesque aliases */

	{ "read",
	  (PyCFunction) pysmdev_handle_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> String\n"
	  "\n"
	  "Reads a buffer of media data." },

	{ "write",
	  (PyCFunction) pysmdev_handle_write_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "write(buffer, size) -> None\n"
	  "\n"
	  "Writes a buffer of media data." },

	{ "seek",
	  (PyCFunction) pysmdev_handle_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the media data." },

	{ "tell",
	  (PyCFunction) pysmdev_handle_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the media data." },

	/* Functions to access the metadata */

	{ "get_media_size",
	  (PyCFunction) pysmdev_handle_get_media_size,
	  METH_NOARGS,
	  "get_media_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the media data." },

	{ "get_bytes_per_sector",
	  (PyCFunction) pysmdev_handle_get_bytes_per_sector,
	  METH_NOARGS,
	  "get_bytes_per_sector() -> Integer or None\n"
	  "\n"
	  "Retrieves the number of bytes per sector." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pysmdev_handle_object_get_set_definitions[] = {

	{ "media_size",
	  (getter) pysmdev_handle_get_media_size,
	  (setter) 0,
	  "The media size.",
	  NULL },

	{ "bytes_per_sector",
	  (getter) pysmdev_handle_get_bytes_per_sector,
	  (setter) 0,
	  "The bytes per sector.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pysmdev_handle_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pysmdev.handle",
	/* tp_basicsize */
	sizeof( pysmdev_handle_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pysmdev_handle_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pysmdev handle object (wraps libsmdev_handle_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pysmdev_handle_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pysmdev_handle_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pysmdev_handle_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new pysmdev handle object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmdev_handle_new(
           void )
{
	static char *function            = "pysmdev_handle_new";
	pysmdev_handle_t *pysmdev_handle = NULL;

	pysmdev_handle = PyObject_New(
	                  struct pysmdev_handle,
	                  &pysmdev_handle_type_object );

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize handle.",
		 function );

		goto on_error;
	}
	if( pysmdev_handle_init(
	     pysmdev_handle ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize handle.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pysmdev_handle );

on_error:
	if( pysmdev_handle != NULL )
	{
		Py_DecRef(
		 (PyObject *) pysmdev_handle );
	}
	return( NULL );
}

/* Creates a new handle object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmdev_handle_new_open(
           PyObject *self PYSMDEV_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pysmdev_handle = NULL;

	PYSMDEV_UNREFERENCED_PARAMETER( self )

	pysmdev_handle = pysmdev_handle_new();

	pysmdev_handle_open(
	 (pysmdev_handle_t *) pysmdev_handle,
	 arguments,
	 keywords );

	return( pysmdev_handle );
}

/* Intializes a handle object
 * Returns 0 if successful or -1 on error
 */
int pysmdev_handle_init(
     pysmdev_handle_t *pysmdev_handle )
{
	static char *function    = "pysmdev_handle_init";
	libcerror_error_t *error = NULL;

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	/* Make sure libsmdev handle is set to NULL
	 */
	pysmdev_handle->handle = NULL;

	if( libsmdev_handle_initialize(
	     &( pysmdev_handle->handle ),
	     &error ) != 1 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize handle.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a handle object
 */
void pysmdev_handle_free(
      pysmdev_handle_t *pysmdev_handle )
{
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pysmdev_handle_free";
	int result                  = 0;

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return;
	}
	if( pysmdev_handle->handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle - missing libsmdev handle.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pysmdev_handle );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libsmdev_handle_free(
	          &( pysmdev_handle->handle ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free handle.",
		 function );

		libcerror_error_free(
		 &error );
	}
	ob_type->tp_free(
	 (PyObject*) pysmdev_handle );
}

/* Signals the handle to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmdev_handle_signal_abort(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments PYSMDEV_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pysmdev_handle_signal_abort";
	int result               = 0;

	PYSMDEV_UNREFERENCED_PARAMETER( arguments )

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pysmdev handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libsmdev_handle_signal_abort(
	          pysmdev_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmdev_handle_open(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pysmdev_handle_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	const char *filename_narrow  = NULL;
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		PyErr_Format(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libsmdev_handle_open_wide(
		          pysmdev_handle->handle,
	                  filename_wide,
		          LIBSMDEV_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pysmdev_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
				   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libsmdev_handle_open(
		          pysmdev_handle->handle,
	                  filename_narrow,
		          LIBSMDEV_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pysmdev_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pysmdev_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   string_object );
#else
		filename_narrow = PyString_AsString(
				   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libsmdev_handle_open(
		          pysmdev_handle->handle,
	                  filename_narrow,
		          LIBSMDEV_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pysmdev_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Closes a handle
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmdev_handle_close(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments PYSMDEV_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pysmdev_handle_close";
	int result               = 0;

	PYSMDEV_UNREFERENCED_PARAMETER( arguments )

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pysmdev handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libsmdev_handle_close(
	          pysmdev_handle->handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close handle.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Reads a buffer of media data
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pysmdev_handle_read_buffer(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pysmdev_handle_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	char *buffer                = NULL;
	ssize_t read_count          = 0;
	int read_size               = -1;

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pysmdev handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|i",
	     keyword_list,
	     &read_size ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( read_size > INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libsmdev_handle_read_buffer(
	              pysmdev_handle->handle,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Reads a buffer of media data at a specific offset
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pysmdev_handle_read_buffer_at_offset(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pysmdev_handle_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	char *buffer                = NULL;
	off64_t read_offset         = 0;
	ssize_t read_count          = 0;
	int read_size               = 0;

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pysmdev handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i|L",
	     keyword_list,
	     &read_size,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( read_size > INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read offset value less than zero.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libsmdev_handle_read_buffer_at_offset(
	              pysmdev_handle->handle,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Writes a buffer of media data
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pysmdev_handle_write_buffer(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pysmdev_handle_write_buffer";
	static char *keyword_list[] = { "buffer", NULL };
	char *buffer                = NULL;
	Py_ssize_t buffer_size      = 0;
	ssize_t write_count         = 0;

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pysmdev handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	buffer = PyBytes_AsString(
	          string_object );

	buffer_size = PyBytes_Size(
	               string_object );
#else
	buffer = PyString_AsString(
	          string_object );

	buffer_size = PyString_Size(
	               string_object );
#endif
	if( ( buffer_size < 0 )
	 || ( buffer_size > (Py_ssize_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument buffer size value out of bounds.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	write_count = libsmdev_handle_write_buffer(
	               pysmdev_handle->handle,
	               (uint8_t *) buffer,
	               (size_t) buffer_size,
	               &error );

	Py_END_ALLOW_THREADS

	if( write_count != (ssize_t) buffer_size )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to write data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Writes a buffer of media data at a specific offset
 * Returns a Python object holding the data if successful or NULL on error
 */
PyObject *pysmdev_handle_write_buffer_at_offset(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pysmdev_handle_write_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	char *buffer                = NULL;
	off64_t write_offset        = 0;
	Py_ssize_t buffer_size      = 0;
	ssize_t write_count         = 0;

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pysmdev handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|L",
	     keyword_list,
	     &string_object,
	     &write_offset ) == 0 )
	{
		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	buffer = PyBytes_AsString(
	          string_object );

	buffer_size = PyBytes_Size(
	               string_object );
#else
	buffer = PyString_AsString(
	          string_object );

	buffer_size = PyString_Size(
	               string_object );
#endif
	if( ( buffer_size < 0 )
	 || ( buffer_size > (Py_ssize_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument buffer size value out of bounds.",
		 function );

		return( NULL );
	}
	if( write_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument write offset value less than zero.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	write_count = libsmdev_handle_write_buffer_at_offset(
	               pysmdev_handle->handle,
	               (uint8_t *) buffer,
	               (size_t) buffer_size,
	               write_offset,
	               &error );

	Py_END_ALLOW_THREADS

	if( write_count != (ssize_t) buffer_size )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to write data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Seeks a certain offset in the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pysmdev_handle_seek_offset(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pysmdev_handle_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid pysmdev handle.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libsmdev_handle_seek_offset(
	          pysmdev_handle->handle,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

 	if( offset == -1 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pysmdev_handle_get_offset(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments PYSMDEV_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pysmdev_handle_get_offset";
	off64_t current_offset   = 0;
	int result               = 0;

	PYSMDEV_UNREFERENCED_PARAMETER( arguments )

	if( pysmdev_handle == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid handle.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libsmdev_handle_get_offset(
	          pysmdev_handle->handle,
	          &current_offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pysmdev_integer_signed_new_from_64bit(
	                  (int64_t) current_offset );

	return( integer_object );
}

