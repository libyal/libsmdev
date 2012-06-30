/*
 * Handle functions
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

#if !defined( _LIBSMDEV_INTERNAL_HANDLE_H )
#define _LIBSMDEV_INTERNAL_HANDLE_H

#include <common.h>
#include <types.h>

#include "libsmdev_array_type.h"
#include "libsmdev_extern.h"
#include "libsmdev_libcerror.h"
#include "libsmdev_libcstring.h"
#include "libsmdev_offset_list.h"
#include "libsmdev_types.h"

#if defined( _MSC_VER ) || defined( __BORLANDC__ )

/* This inclusion is needed otherwise some linkers
 * mess up exporting the legacy functions
 */
#include "libsmdev_legacy.h"
#endif

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libsmdev_internal_handle libsmdev_internal_handle_t;

struct libsmdev_internal_handle
{
	/* The device filename
	 */
	libcstring_system_character_t *filename;

	/* The device filename size
	 */
	size_t filename_size;

#if defined( WINAPI )
	/* The device file handle
	 */
	HANDLE file_handle;
#else
	/* The device file descriptor
	 */
	int file_descriptor;
#endif

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
	libsmdev_array_t *tracks_array;

	/* The sessions array
	 */
	libsmdev_array_t *sessions_array;

	/* The lead-outs array
	 */
	libsmdev_array_t *lead_outs_array;

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

	/* The read/write errors offset list
	 */
	libsmdev_offset_list_t *errors;

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
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBSMDEV_EXTERN \
int libsmdev_handle_open_wide(
     libsmdev_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
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
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBSMDEV_EXTERN \
ssize_t libsmdev_handle_write_buffer(
         libsmdev_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

#if defined( WINAPI ) && ( WINVER < 0x0500 )
BOOL libsmdev_SetFilePointerEx(
      HANDLE file_handle,
      LARGE_INTEGER distance_to_move_large_integer,
      LARGE_INTEGER *new_file_pointer_large_integer,
      DWORD move_method );
#endif

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
#endif

int libsmdev_file_exists(
     const char *filename,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libsmdev_file_exists_wide(
     const wchar_t *filename,
     libcerror_error_t **error );
#endif

#if defined( __cplusplus )
}
#endif

#endif

