/*
 * Meta data functions
 *
 * Copyright (c) 2008-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBSMDEV_METADATA_H )
#define _LIBSMDEV_METADATA_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libsmdev_extern.h"
#include "libsmdev_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBSMDEV_EXTERN int libsmdev_handle_get_media_size(
                     libsmdev_handle_t *handle,
                     size64_t *media_size,
                     liberror_error_t **error );

LIBSMDEV_EXTERN int libsmdev_handle_get_bytes_per_sector(
                     libsmdev_handle_t *handle,
                     uint32_t *bytes_per_sector,
                     liberror_error_t **error );

int libsmdev_internal_handle_determine_media_information(
     libsmdev_internal_handle_t *internal_handle,
     liberror_error_t **error );

LIBSMDEV_EXTERN int libsmdev_handle_get_media_type(
                     libsmdev_handle_t *handle,
                     uint8_t *media_type,
                     liberror_error_t **error );

LIBSMDEV_EXTERN int libsmdev_handle_get_bus_type(
                     libsmdev_handle_t *handle,
                     uint8_t *bus_type,
                     liberror_error_t **error );

LIBSMDEV_EXTERN int libsmdev_handle_get_information_value(
                     libsmdev_handle_t *handle,
                     const uint8_t *information_value_identifier,
                     size_t information_value_identifier_length,
                     uint8_t *information_value,
                     size_t information_value_size,
                     liberror_error_t **error );

LIBSMIO_EXTERN int libsmio_device_get_amount_of_error_retries(
                    libsmio_handle_t *handle,
                    uint8_t *amount_of_error_retries,
                    liberror_error_t **error );

LIBSMIO_EXTERN int libsmio_device_set_amount_of_error_retries(
                    libsmio_handle_t *handle,
                    uint8_t amount_of_error_retries,
                    liberror_error_t **error );

LIBSMIO_EXTERN int libsmio_device_get_error_granularity(
                    libsmio_handle_t *handle,
                    size_t *error_granularity,
                    liberror_error_t **error );

LIBSMIO_EXTERN int libsmio_device_set_error_granularity(
                    libsmio_handle_t *handle,
                    size_t *error_granularity,
                    liberror_error_t **error );

LIBSMIO_EXTERN int libsmio_device_get_error_flags(
                    libsmio_handle_t *handle,
                    uint8_t *error_flags,
                    liberror_error_t **error );

LIBSMIO_EXTERN int libsmio_device_set_error_flags(
                    libsmio_handle_t *handle,
                    uint8_t error_flags,
                    liberror_error_t **error );

#ifdef TODO

int libsmio_device_get_amount_of_read_errors(
     intptr_t *io_handle,
     int *amount_of_errors,
     liberror_error_t **error );

int libsmio_device_get_read_error(
     intptr_t *io_handle,
     int index,
     off64_t *offset,
     size64_t *size,
     liberror_error_t **error );

int libsmio_device_add_read_error(
     intptr_t *io_handle,
     off64_t offset,
     size64_t size,
     liberror_error_t **error );

#endif /* TODO */

#if defined( __cplusplus )
}
#endif

#endif

