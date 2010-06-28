/*
 * Offset list
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBSMDEV_OFFSET_LIST_H )
#define _LIBSMDEV_OFFSET_LIST_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libsmdev_list_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmdev_offset_list_values libsmdev_offset_list_values_t;

struct libsmdev_offset_list_values
{
	/* The offset
	 */
	off64_t offset;

	/* The size
	 */
	size64_t size;
};

int libsmdev_offset_list_values_free(
     intptr_t *values,
     liberror_error_t **error );

int libsmdev_offset_list_values_compare(
     intptr_t *first,
     intptr_t *second,
     liberror_error_t **error );

int libsmdev_offset_list_add_offset(
     libsmdev_list_t *offset_list,
     off64_t offset,
     size64_t size,
     liberror_error_t **error );

int libsmdev_offset_list_get_offset(
     libsmdev_list_t *offset_list,
     int index,
     off64_t *offset,
     size64_t *size,
     liberror_error_t **error );

int libsmdev_offset_list_get_offset_by_value(
     libsmdev_list_t *offset_list,
     off64_t offset_value,
     off64_t *offset,
     size64_t *size,
     liberror_error_t **error );

int libsmdev_offset_list_range_is_present(
     libsmdev_list_t *offset_list,
     off64_t offset,
     size64_t size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

