/*
 *  Metadata functions for the Python object definition of the libsmdev handle
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
#include <types.h>

#include "pysmdev_error.h"
#include "pysmdev_handle.h"
#include "pysmdev_integer.h"
#include "pysmdev_libcerror.h"
#include "pysmdev_libsmdev.h"
#include "pysmdev_metadata.h"
#include "pysmdev_python.h"
#include "pysmdev_unused.h"

/* Retrieves the size of the media data
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pysmdev_handle_get_media_size(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments PYSMDEV_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pysmdev_handle_get_media_size";
	size64_t media_size      = 0;
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

	result = libsmdev_handle_get_media_size(
	          pysmdev_handle->handle,
	          &media_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve media size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pysmdev_integer_unsigned_new_from_64bit(
	                  (uint64_t) media_size );

	return( integer_object );
}

/* Retrieves the number of bytes per sector
 * Returns a Python object holding the offset if successful or NULL on error
 */
PyObject *pysmdev_handle_get_bytes_per_sector(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments PYSMDEV_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error  = NULL;
	PyObject *integer_object  = NULL;
	static char *function     = "pysmdev_handle_get_bytes_per_sector";
	uint32_t bytes_per_sector = 0;
	int result                = 0;

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

	result = libsmdev_handle_get_bytes_per_sector(
	          pysmdev_handle->handle,
	          &bytes_per_sector,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pysmdev_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pysmdev_integer_unsigned_new_from_64bit(
	                  (uint64_t) bytes_per_sector );

	return( integer_object );
}

