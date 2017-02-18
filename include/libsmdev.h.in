/*
 * Library to access and read the storage media (SM) devices
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

#if !defined( _LIBSMDEV_H )
#define _LIBSMDEV_H

#include <libsmdev/codepage.h>
#include <libsmdev/definitions.h>
#include <libsmdev/error.h>
#include <libsmdev/extern.h>
#include <libsmdev/features.h>
#include <libsmdev/types.h>

#include <stdio.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBSMDEV_EXTERN \
const char *libsmdev_get_version(
             void );

/* Returns the access flags for reading
 */
LIBSMDEV_EXTERN \
int libsmdev_get_access_flags_read(
     void );

/* Returns the access flags for reading and writing
 */
LIBSMDEV_EXTERN \
int libsmdev_get_access_flags_read_write(
     void );

/* Returns the access flags for writing
 */
LIBSMDEV_EXTERN \
int libsmdev_get_access_flags_write(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_get_codepage(
     int *codepage,
     libsmdev_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_set_codepage(
     int codepage,
     libsmdev_error_t **error );

/* Determines if a file is a device
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_check_device(
     const char *filename,
     libsmdev_error_t **error );

#if defined( LIBSMDEV_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file is a device
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_check_device_wide(
     const wchar_t *filename,
     libsmdev_error_t **error );

#endif /* defined( LIBSMDEV_HAVE_WIDE_CHARACTER_TYPE ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBSMDEV_EXTERN \
void libsmdev_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_notify_set_stream(
     FILE *stream,
     libsmdev_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_notify_stream_open(
     const char *filename,
     libsmdev_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_notify_stream_close(
     libsmdev_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBSMDEV_EXTERN \
void libsmdev_error_free(
      libsmdev_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_error_fprint(
     libsmdev_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_error_sprint(
     libsmdev_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_error_backtrace_fprint(
     libsmdev_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_error_backtrace_sprint(
     libsmdev_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Handle functions
 * ------------------------------------------------------------------------- */

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_initialize(
     libsmdev_handle_t **handle,
     libsmdev_error_t **error );

/* Frees a handle
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_free(
     libsmdev_handle_t **handle,
     libsmdev_error_t **error );

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_signal_abort(
     libsmdev_handle_t *handle,
     libsmdev_error_t **error );

/* Opens a storage media device
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_open(
     libsmdev_handle_t *handle,
     const char *filename,
     int access_flags,
     libsmdev_error_t **error );

#if defined( LIBSMDEV_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a storage media device
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_open_wide(
     libsmdev_handle_t *handle,
     const wchar_t *filename,
     int access_flags,
     libsmdev_error_t **error );

#endif /* defined( LIBSMDEV_HAVE_WIDE_CHARACTER_TYPE ) */

/* Closes the handle
 * Returns the 0 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_close(
     libsmdev_handle_t *handle,
     libsmdev_error_t **error );

/* Reads a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBSMDEV_EXTERN \
ssize_t libsmdev_handle_read_buffer(
         libsmdev_handle_t *handle,
         uint8_t *buffer,
         size_t buffer_size,
         libsmdev_error_t **error );

/* Reads a buffer at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBSMDEV_EXTERN \
ssize_t libsmdev_handle_read_buffer_at_offset(
         libsmdev_handle_t *handle,
         uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libsmdev_error_t **error );

/* Writes a buffer
 * Returns the number of bytes written or -1 on error
 */
LIBSMDEV_EXTERN \
ssize_t libsmdev_handle_write_buffer(
         libsmdev_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         libsmdev_error_t **error );

/* Writes a buffer at a specific offset
 * Returns the number of bytes written or -1 on error
 */
LIBSMDEV_EXTERN \
ssize_t libsmdev_handle_write_buffer_at_offset(
         libsmdev_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libsmdev_error_t **error );

/* Seeks an offset
 * Returns the offset if seek is successful or -1 on error
 */
LIBSMDEV_EXTERN \
off64_t libsmdev_handle_seek_offset(
         libsmdev_handle_t *handle,
         off64_t offset,
         int whence,
         libsmdev_error_t **error );

/* Retrieves the current offset of the (media) data
 * Returns the offset if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_offset(
     libsmdev_handle_t *handle,
     off64_t *offset,
     libsmdev_error_t **error );

/* Retrieves the filename size of the device
 * The filename size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_filename_size(
     libsmdev_handle_t *handle,
     size_t *filename_size,
     libsmdev_error_t **error );

/* Retrieves the filename of the device
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_filename(
     libsmdev_handle_t *handle,
     char *filename,
     size_t filename_size,
     libsmdev_error_t **error );

#if defined( LIBSMDEV_HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the device
 * The filename size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_filename_size_wide(
     libsmdev_handle_t *handle,
     size_t *filename_size,
     libsmdev_error_t **error );

/* Retrieves the filename of the device
 * The filename size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_filename_wide(
     libsmdev_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libsmdev_error_t **error );

#endif /* defined( LIBSMDEV_HAVE_WIDE_CHARACTER_TYPE ) */

/* -------------------------------------------------------------------------
 * Meta data functions
 * ------------------------------------------------------------------------- */

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_media_size(
     libsmdev_handle_t *handle,
     size64_t *media_size,
     libsmdev_error_t **error );

/* Retrieves the number of bytes per sector
 * Returns the 1 if succesful, 0 if not or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_bytes_per_sector(
     libsmdev_handle_t *handle,
     uint32_t *bytes_per_sector,
     libsmdev_error_t **error );

/* Retrieves the media type
 * Returns the 1 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_media_type(
     libsmdev_handle_t *handle,
     uint8_t *media_type,
     libsmdev_error_t **error );

/* Retrieves the bus type
 * Returns the 1 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_bus_type(
     libsmdev_handle_t *handle,
     uint8_t *bus_type,
     libsmdev_error_t **error );

/* Retrieves an UTF-8 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_utf8_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_size,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     libsmdev_error_t **error );

/* Retrieves an UTF-16 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_utf16_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_size,
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     libsmdev_error_t **error );

/* Retrieves the number of sessions
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_sessions(
     libsmdev_handle_t *handle,
     int *number_of_sessions,
     libsmdev_error_t **error );

/* Retrieves a session
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_session(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libsmdev_error_t **error );

/* Retrieves the number of tracks
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_tracks(
     libsmdev_handle_t *handle,
     int *number_of_tracks,
     libsmdev_error_t **error );

/* Retrieves a track
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_track(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     uint8_t *type,
     libsmdev_error_t **error );

/* Retrieves the number of read/write error retries
 * Returns the 1 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t *number_of_error_retries,
     libsmdev_error_t **error );

/* Sets the number of read/write error retries
 * Returns the 1 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_set_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t number_of_error_retries,
     libsmdev_error_t **error );

/* Retrieves the read/write error granularity
 * A value of 0 represents an error granularity of the entire buffer being read/written
 * Returns the 1 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_error_granularity(
     libsmdev_handle_t *handle,
     size_t *error_granularity,
     libsmdev_error_t **error );

/* Sets the read/write error granularity
 * A value of 0 represents an error granularity of the entire buffer being read/written
 * Returns the 1 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_set_error_granularity(
     libsmdev_handle_t *handle,
     size_t error_granularity,
     libsmdev_error_t **error );

/* Retrieves the read/write error flags
 * Returns the 1 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_error_flags(
     libsmdev_handle_t *handle,
     uint8_t *error_flags,
     libsmdev_error_t **error );

/* Sets the read/write error flags
 * Returns the 1 if succesful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_set_error_flags(
     libsmdev_handle_t *handle,
     uint8_t error_flags,
     libsmdev_error_t **error );

/* Retrieves the number of read/write errors
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_number_of_errors(
     libsmdev_handle_t *handle,
     int *number_of_errors,
     libsmdev_error_t **error );

/* Retrieves a read/write error
 * Returns 1 if successful or -1 on error
 */
LIBSMDEV_EXTERN \
int libsmdev_handle_get_error(
     libsmdev_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     libsmdev_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMDEV_H ) */

