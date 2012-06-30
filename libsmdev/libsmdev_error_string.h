/*
 * Error string functions
 *
 * Copyright (c) 2010-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBSMDEV_ERROR_STRING_H )
#define _LIBSMDEV_ERROR_STRING_H

#include <common.h>
#include <types.h>

#include "libsmdev_libcerror.h"
#include "libsmdev_libcstring.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define LIBSMDEV_ERROR_STRING_DEFAULT_SIZE	128

int libsmdev_error_string_copy_from_error_number(
     libcstring_system_character_t *error_string,
     size_t error_string_size,
     int error_number,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

