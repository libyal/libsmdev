/*
 * Offset list
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

#if !defined( _LIBSMDEV_OFFSET_LIST_H )
#define _LIBSMDEV_OFFSET_LIST_H

#include <common.h>
#include <types.h>

#include "libsmdev_libcerror.h"
#include "libsmdev_list_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmdev_offset_list_value libsmdev_offset_list_value_t;

struct libsmdev_offset_list_value
{
	/* The offset
	 */
	off64_t offset;

	/* The size
	 */
	size64_t size;
};

typedef struct libsmdev_offset_list libsmdev_offset_list_t;

struct libsmdev_offset_list
{
	/* The number of elements
	 */
	int number_of_elements;

	/* The first element
	 */
	libsmdev_list_element_t *first_element;

	/* The last element
	 */
	libsmdev_list_element_t *last_element;

	/* The current list element
	 */
	libsmdev_list_element_t *current_element;

	/* The current list element index
	 */
	int current_element_index;
};

int libsmdev_offset_list_value_initialize(
     libsmdev_offset_list_value_t **offset_list_value,
     libcerror_error_t **error );

int libsmdev_offset_list_value_free(
     libsmdev_offset_list_value_t **offset_list_value,
     libcerror_error_t **error );

int libsmdev_offset_list_value_clone(
     libsmdev_offset_list_value_t **destination_offset_list_value,
     libsmdev_offset_list_value_t *source_offset_list_value,
     libcerror_error_t **error );

int libsmdev_offset_list_initialize(
     libsmdev_offset_list_t **offset_list,
     libcerror_error_t **error );

int libsmdev_offset_list_free(
     libsmdev_offset_list_t **offset_list,
     libcerror_error_t **error );

int libsmdev_offset_list_empty(
     libsmdev_offset_list_t *offset_list,
     libcerror_error_t **error );

int libsmdev_offset_list_clone(
     libsmdev_offset_list_t **destination_offset_list,
     libsmdev_offset_list_t *source_offset_list,
     libcerror_error_t **error );

int libsmdev_offset_list_get_number_of_elements(
     libsmdev_offset_list_t *offset_list,
     int *number_of_elements,
     libcerror_error_t **error );

int libsmdev_offset_list_append_offset(
     libsmdev_offset_list_t *offset_list,
     off64_t offset,
     size64_t size,
     uint8_t merge_ranges,
     libcerror_error_t **error );

int libsmdev_offset_list_get_offset(
     libsmdev_offset_list_t *offset_list,
     int index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error );

int libsmdev_offset_list_get_offset_by_value(
     libsmdev_offset_list_t *offset_list,
     off64_t offset_value,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error );

int libsmdev_offset_list_range_is_present(
     libsmdev_offset_list_t *offset_list,
     off64_t offset,
     size64_t size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

