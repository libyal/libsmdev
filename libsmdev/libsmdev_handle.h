/*
 * Handle functions
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

#if !defined( _LIBSMDEV_INTERNAL_HANDLE_H )
#define _LIBSMDEV_INTERNAL_HANDLE_H

#include <common.h>
#include <types.h>

#include "libsmdev_extern.h"
#include "libsmdev_libcdata.h"
#include "libsmdev_libcerror.h"
#include "libsmdev_libcfile.h"
#include "libsmdev_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmdev_internal_handle libsmdev_internal_handle_t;

struct libsmdev_internal_handle
{
	/* The device filename
	 */
	system_character_t *filename;

	/* The device filename size
	 */
	size_t filename_size;

	/* The device file
	 */
	libcfile_file_t *device_file;

	/* The current offset
	 */
	off64_t offset;

	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* Value to indicate the bytes per sector value was set
	 */
	uint8_t bytes_per_sector_set;

	/* The media size
	 */
	size64_t media_size;

	/* Value to indicate the media size value was set
	 */
	uint8_t media_size_set;

	/* The bus type
	 */
	uint8_t bus_type;

	/* The device type
	 */
	uint8_t device_type;

	/* Value to indicate if the device is removable
	 */
	uint8_t removable;

	/* The vendor string
	 */
	uint8_t vendor[ 64 ];

	/* The model string
	 */
	uint8_t model[ 64 ];

	/* The serial number string
	 */
	uint8_t serial_number[ 64 ];

	/* The tracks array
	 */
	libcdata_array_t *tracks_array;

	/* The sessions array
	 */
	libcdata_array_t *sessions_array;

	/* The lead-outs array
	 */
	libcdata_array_t *lead_outs_array;

	/* Value to indicate the media information values were set
	 */
	uint8_t media_information_set;

	/* The number of read/write error retries
	 */
	uint8_t number_of_error_retries;

	/* The error granularity
	 */
	size_t error_granularity;

	/* The read/write error flags
	 */
	uint8_t error_flags;

	/* The read/write errors range list
	 */
	libcdata_range_list_t *errors_range_list;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

LIBSMDEV_EXTERN \
int libsmdev_handle_initialize(
     libsmdev_handle_t **handle,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_free(
     libsmdev_handle_t **handle,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_signal_abort(
     libsmdev_handle_t *handle,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_open(
     libsmdev_handle_t *handle,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBSMDEV_EXTERN \
int libsmdev_handle_open_wide(
     libsmdev_handle_t *handle,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );
#endif

LIBSMDEV_EXTERN \
int libsmdev_handle_close(
     libsmdev_handle_t *handle,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
ssize_t libsmdev_handle_read_buffer(
         libsmdev_handle_t *handle,
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBSMDEV_EXTERN \
ssize_t libsmdev_handle_read_buffer_at_offset(
         libsmdev_handle_t *handle,
         uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBSMDEV_EXTERN \
ssize_t libsmdev_handle_write_buffer(
         libsmdev_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBSMDEV_EXTERN \
ssize_t libsmdev_handle_write_buffer_at_offset(
         libsmdev_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBSMDEV_EXTERN \
off64_t libsmdev_handle_seek_offset(
         libsmdev_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_offset(
     libsmdev_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_filename_size(
     libsmdev_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_filename(
     libsmdev_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error );

int libsmdev_handle_set_filename(
     libsmdev_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBSMDEV_EXTERN \
int libsmdev_handle_get_filename_size_wide(
     libsmdev_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_filename_wide(
     libsmdev_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error );

int libsmdev_handle_set_filename_wide(
     libsmdev_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

int libsmdev_internal_handle_determine_media_information(
     libsmdev_internal_handle_t *internal_handle,
     libcerror_error_t **error );

int libsmdev_handle_append_session(
     libsmdev_internal_handle_t *internal_handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error );

int libsmdev_handle_append_lead_out(
     libsmdev_internal_handle_t *internal_handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error );

int libsmdev_handle_append_track(
     libsmdev_internal_handle_t *internal_handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     uint8_t type,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_media_size(
     libsmdev_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_bytes_per_sector(
     libsmdev_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_media_type(
     libsmdev_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_bus_type(
     libsmdev_handle_t *handle,
     uint8_t *bus_type,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_utf8_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_utf16_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_sessions(
     libsmdev_handle_t *handle,
     int *number_of_sessions,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_session(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_tracks(
     libsmdev_handle_t *handle,
     int *number_of_tracks,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_track(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     uint8_t *type,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t *number_of_error_retries,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_set_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t number_of_error_retries,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_error_granularity(
     libsmdev_handle_t *handle,
     size_t *error_granularity,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_set_error_granularity(
     libsmdev_handle_t *handle,
     size_t error_granularity,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_error_flags(
     libsmdev_handle_t *handle,
     uint8_t *error_flags,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_set_error_flags(
     libsmdev_handle_t *handle,
     uint8_t error_flags,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_errors(
     libsmdev_handle_t *handle,
     int *number_of_errors,
     libcerror_error_t **error );

LIBSMDEV_EXTERN \
int libsmdev_handle_get_error(
     libsmdev_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMDEV_INTERNAL_HANDLE_H ) */

