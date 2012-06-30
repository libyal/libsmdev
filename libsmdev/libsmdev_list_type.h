/*
 * List type functions
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

#if !defined( _LIBSMDEV_LIST_TYPE_H )
#define _LIBSMDEV_LIST_TYPE_H

#include <common.h>
#include <types.h>

#include "libsmdev_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The list comparison definitions
 */
enum LIBSMDEV_LIST_COMPARE_DEFINITIONS
{
	/* The first value is less than the second value
	 */
        LIBSMDEV_LIST_COMPARE_LESS,

	/* The first and second values are equal
	 */
        LIBSMDEV_LIST_COMPARE_EQUAL,

	/* The first value is greater than the second value
	 */
        LIBSMDEV_LIST_COMPARE_GREATER
};

/* The list insert flag definitions
 */
enum LIBSMDEV_LIST_INSERT_FLAGS
{
	/* Allow duplicate entries
	 */
	LIBSMDEV_LIST_INSERT_FLAG_NON_UNIQUE_ENTRIES	= 0x00,

	/* Only allow unique entries, no duplicates
	 */
	LIBSMDEV_LIST_INSERT_FLAG_UNIQUE_ENTRIES	= 0x01,
};

typedef struct libsmdev_list_element libsmdev_list_element_t;

struct libsmdev_list_element
{
	/* The previous list element
	 */
	libsmdev_list_element_t *previous_element;

	/* The next list element
	 */
	libsmdev_list_element_t *next_element;

	/* The value
	 */
	intptr_t *value;
};

typedef struct libsmdev_list libsmdev_list_t;

struct libsmdev_list
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
};

int libsmdev_list_element_initialize(
     libsmdev_list_element_t **element,
     libcerror_error_t **error );

int libsmdev_list_element_free(
     libsmdev_list_element_t **element,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libsmdev_list_element_get_value(
     libsmdev_list_element_t *element,
     intptr_t **value,
     libcerror_error_t **error );

int libsmdev_list_element_set_value(
     libsmdev_list_element_t *element,
     intptr_t *value,
     libcerror_error_t **error );

int libsmdev_list_initialize(
     libsmdev_list_t **list,
     libcerror_error_t **error );

int libsmdev_list_free(
     libsmdev_list_t **list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libsmdev_list_empty(
     libsmdev_list_t *list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libsmdev_list_clone(
     libsmdev_list_t **destination_list,
     libsmdev_list_t *source_list,
     int (*value_free_function)(
            intptr_t **value,
            libcerror_error_t **error ),
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            libcerror_error_t **error ),
     libcerror_error_t **error );

int libsmdev_list_get_number_of_elements(
     libsmdev_list_t *list,
     int *number_of_elements,
     libcerror_error_t **error );

int libsmdev_list_get_element_by_index(
     libsmdev_list_t *list,
     int list_element_index,
     libsmdev_list_element_t **element,
     libcerror_error_t **error );

int libsmdev_list_get_value_by_index(
     libsmdev_list_t *list,
     int list_element_index,
     intptr_t **value,
     libcerror_error_t **error );

int libsmdev_list_prepend_element(
     libsmdev_list_t *list,
     libsmdev_list_element_t *element,
     libcerror_error_t **error );

int libsmdev_list_prepend_value(
     libsmdev_list_t *list,
     intptr_t *value,
     libcerror_error_t **error );

int libsmdev_list_append_element(
     libsmdev_list_t *list,
     libsmdev_list_element_t *element,
     libcerror_error_t **error );

int libsmdev_list_append_value(
     libsmdev_list_t *list,
     intptr_t *value,
     libcerror_error_t **error );

int libsmdev_list_insert_element(
     libsmdev_list_t *list,
     libsmdev_list_element_t *element,
     int (*value_compare_function)(
            intptr_t *first,
            intptr_t *second,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcerror_error_t **error );

int libsmdev_list_insert_value(
     libsmdev_list_t *list,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first,
            intptr_t *second,
            libcerror_error_t **error ),
     uint8_t insert_flags,
     libcerror_error_t **error );

int libsmdev_list_remove_element(
     libsmdev_list_t *list,
     libsmdev_list_element_t *element,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

