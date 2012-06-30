/*
 * Legacy functions
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

#if !defined( _LIBSMDEV_LEGACY_H )
#define _LIBSMDEV_LEGACY_H

#include <common.h>
#include <types.h>

#include "libsmdev_extern.h"
#include "libsmdev_libcerror.h"
#include "libsmdev_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBSMDEV )

LIBSMDEV_EXTERN \
int libsmdev_handle_get_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *information_value_identifier,
     size_t information_value_identifier_length,
     uint8_t *information_value,
     size_t information_value_size,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_amount_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t *amount_of_error_retries,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_set_amount_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t amount_of_error_retries,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBSMDEV ) */

#if defined( __cplusplus )
}
#endif

#endif

