/*
 * Metadata functions for the Python object definition of the libsmdev handle
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

#if !defined( _PYSMDEV_METADATA_H )
#define _PYSMDEV_METADATA_H

#include <common.h>
#include <types.h>

#include "pysmdev_handle.h"
#include "pysmdev_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

PyObject *pysmdev_handle_get_media_size(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments );

PyObject *pysmdev_handle_get_bytes_per_sector(
           pysmdev_handle_t *pysmdev_handle,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYSMDEV_METADATA_H ) */

