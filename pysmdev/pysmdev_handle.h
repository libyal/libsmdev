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

#if !defined( _PYSMDEV_HANDLE_H )
#define _PYSMDEV_HANDLE_H

#include <common.h>
#include <types.h>

#include "pysmdev_libcerror.h"
#include "pysmdev_libsmdev.h"
#include "pysmdev_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pysmdev_handle pysmdev_handle_t;

struct pysmdev_handle
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libsmdev handle
	 */
	libsmdev_handle_t *handle;
};

extern PyMethodDef pysmdev_handle_object_methods[];
extern PyTypeObject pysmdev_handle_type_object;

PyObject *pysmdev_handle_new(
           void );

PyObject *pysmdev_handle_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pysmdev_handle_init(
     pysmdev_handle_t *pysmdev_handle );

void pysmdev_handle_free(
      pysmdev_handle_t *pysmdev_handle );

PyObject *pysmdev_handle_signal_abort(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments );

PyObject *pysmdev_handle_open(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmdev_handle_close(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments );

PyObject *pysmdev_handle_read_buffer(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmdev_handle_read_buffer_at_offset(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmdev_handle_write_buffer(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmdev_handle_write_buffer_at_offset(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmdev_handle_seek_offset(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pysmdev_handle_get_offset(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif

