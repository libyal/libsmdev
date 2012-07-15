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

#include <common.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_SYS_STAT_H )
#include <sys/stat.h>
#endif

#if defined( HAVE_FCNTL_H )
#include <fcntl.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#include "libsmdev_array_type.h"
#include "libsmdev_definitions.h"
#include "libsmdev_error_string.h"
#include "libsmdev_handle.h"
#include "libsmdev_libcerror.h"
#include "libsmdev_libclocale.h"
#include "libsmdev_libcnotify.h"
#include "libsmdev_libcstring.h"
#include "libsmdev_libuna.h"
#include "libsmdev_metadata.h"
#include "libsmdev_offset_list.h"
#include "libsmdev_sector_range.h"
#include "libsmdev_track_value.h"
#include "libsmdev_types.h"

/* The definition of POSIX_FADV_SEQUENTIAL seems to be missing from fcntl.h
 * on some versions of Linux
 */
#if defined( HAVE_POSIX_FADVISE ) && !defined( POSIX_FADV_SEQUENTIAL ) 
#define POSIX_FADV_SEQUENTIAL		2
#endif

/* Initializes the handle
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_initialize(
     libsmdev_handle_t **handle,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_initialize";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle value already set.",
		 function );

		return( -1 );
	}
	internal_handle = memory_allocate_structure(
	                   libsmdev_internal_handle_t );

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_handle,
	     0,
	     sizeof( libsmdev_internal_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear handle.",
		 function );

		memory_free(
		 internal_handle );

		return( -1 );
	}
	if( libsmdev_array_initialize(
	     &( internal_handle->tracks_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create tracks array.",
		 function );

		goto on_error;
	}
	if( libsmdev_array_initialize(
	     &( internal_handle->sessions_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sessions array.",
		 function );

		goto on_error;
	}
	if( libsmdev_array_initialize(
	     &( internal_handle->lead_outs_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create lead-outs array.",
		 function );

		goto on_error;
	}
	if( libsmdev_offset_list_initialize(
	     &( internal_handle->errors ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create errors offset list.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
	internal_handle->file_handle             = INVALID_HANDLE_VALUE;
#else
	internal_handle->file_descriptor         = -1;
#endif
	internal_handle->number_of_error_retries = 2;

	*handle = (libsmdev_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		if( internal_handle->lead_outs_array != NULL )
		{
			libsmdev_array_free(
			 &( internal_handle->lead_outs_array ),
			 NULL,
			 NULL );
		}
		if( internal_handle->sessions_array != NULL )
		{
			libsmdev_array_free(
			 &( internal_handle->sessions_array ),
			 NULL,
			 NULL );
		}
		if( internal_handle->tracks_array != NULL )
		{
			libsmdev_array_free(
			 &( internal_handle->tracks_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 internal_handle );
	}
	return( -1 );
}

/* Frees the handle
 * Returns 1 if succesful or -1 on error
 */
int libsmdev_handle_free(
     libsmdev_handle_t **handle,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_internal_handle_free";
	int result                                  = 1;

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		internal_handle = (libsmdev_internal_handle_t *) *handle;
		*handle         = NULL;

#if defined( WINAPI )
		if( internal_handle->file_handle != INVALID_HANDLE_VALUE )
#else
		if( internal_handle->file_descriptor != -1 )
#endif
		{
			if( libsmdev_handle_close(
			     *handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close handle.",
				 function );

				result = -1;
			}
		}
		if( internal_handle->filename != NULL )
		{
			memory_free(
			 internal_handle->filename );
		}
		if( libsmdev_array_free(
		     &( internal_handle->tracks_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_track_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free tracks array.",
			 function );

			result = -1;
		}
		if( libsmdev_array_free(
		     &( internal_handle->sessions_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_sector_range_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sessions array.",
			 function );

			result = -1;
		}
		if( libsmdev_array_free(
		     &( internal_handle->lead_outs_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_sector_range_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free lead-outs array.",
			 function );

			result = -1;
		}
		if( libsmdev_offset_list_free(
		     &( internal_handle->errors ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free errors offset list.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_handle );
	}
	return( result );
}

/* Signals the handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_signal_abort(
     libsmdev_handle_t *handle,
     libcerror_error_t **error )
{
	static char *function = "libsmdev_handle_signal_abort";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	( (libsmdev_internal_handle_t *) handle )->abort = 1;

	return( 1 );
}

/* Opens a storage media device
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_open(
     libsmdev_handle_t *handle,
     char * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_open";
	size64_t media_size                         = 0;
	size_t filename_length                      = 0;

#if defined( WINAPI )
	DWORD error_code                            = 0;
	DWORD file_io_access_flags                  = 0;
	DWORD file_io_creation_flags                = 0;
#else
	int file_io_flags                           = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - name already exists.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of filenames value out of bounds.",
		 function );

		return( -1 );
	}
	if( number_of_filenames != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: currently only one device file supported.",
		 function );

		return( -1 );
	}
	if( filenames[ 0 ] == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing device filename.",
		 function );

		return( -1 );
	}
	if( libsmdev_array_resize(
	     internal_handle->tracks_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_track_value_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty tracks array.",
		 function );

		goto on_error;
	}
	if( libsmdev_array_resize(
	     internal_handle->sessions_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty sessions array.",
		 function );

		goto on_error;
	}
	if( libsmdev_array_resize(
	     internal_handle->lead_outs_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty lead-outs array.",
		 function );

		goto on_error;
	}
	if( libsmdev_offset_list_empty(
	     internal_handle->errors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty errors offset list.",
		 function );

		goto on_error;
	}
	filename_length = libcstring_narrow_string_length(
	                   filenames[ 0 ] );

	if( libsmdev_handle_set_filename(
	     handle,
	     filenames[ 0 ],
	     filename_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in handle.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
	if( ( ( access_flags & LIBSMDEV_ACCESS_FLAG_READ ) != 0 )
	 && ( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 ) )
	{
		file_io_access_flags   = GENERIC_WRITE | GENERIC_READ;
		file_io_creation_flags = OPEN_ALWAYS;
	}
	else if( ( access_flags & LIBSMDEV_ACCESS_FLAG_READ ) != 0 )
	{
		file_io_access_flags   = GENERIC_READ;
		file_io_creation_flags = OPEN_EXISTING;
	}
	else if( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 )
	{
		file_io_access_flags   = GENERIC_WRITE;
		file_io_creation_flags = OPEN_ALWAYS;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		goto on_error;
	}
	if( ( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 )
	 && ( ( access_flags & LIBSMDEV_ACCESS_FLAG_TRUNCATE ) != 0 ) )
	{
		file_io_creation_flags = TRUNCATE_EXISTING;
	}
	if( internal_handle->file_handle == INVALID_HANDLE_VALUE )
	{
		/* Must use CreateFileA here because filename is a 
		 * narrow character string and CreateFile is dependent
		 * on UNICODE directives
		 */
		internal_handle->file_handle = CreateFileA(
		                                (LPCSTR) filenames[ 0 ],
		                                file_io_access_flags,
		                                0,
		                                NULL,
		                                file_io_creation_flags,
		                                FILE_ATTRIBUTE_NORMAL,
		                                NULL );

		if( internal_handle->file_handle == INVALID_HANDLE_VALUE )
		{
			error_code = GetLastError();

			switch( error_code )
			{
				case ERROR_ACCESS_DENIED:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_ACCESS_DENIED,
					 "%s: access denied to device: %s.",
					 function,
					 filenames[ 0 ] );

					break;

				case ERROR_FILE_NOT_FOUND:
				case ERROR_PATH_NOT_FOUND:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_INVALID_RESOURCE,
					 "%s: no such file: %s.",
					 function,
					 filenames[ 0 ] );

					break;

				default:
					libcerror_system_set_error(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 error_code,
					 "%s: unable to open file: %s.",
					 function,
					 filenames[ 0 ] );

					break;
			}
			goto on_error;
		}
	}
#else
	if( ( ( access_flags & LIBSMDEV_ACCESS_FLAG_READ ) != 0 )
	 && ( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 ) )
	{
		file_io_flags = O_RDWR | O_CREAT;
	}
	else if( ( access_flags & LIBSMDEV_ACCESS_FLAG_READ ) != 0 )
	{
		file_io_flags = O_RDONLY;
	}
	else if( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 )
	{
		file_io_flags = O_WRONLY | O_CREAT;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		goto on_error;
	}
	if( ( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 )
	 && ( ( access_flags & LIBSMDEV_ACCESS_FLAG_TRUNCATE ) != 0 ) )
	{
		file_io_flags |= O_TRUNC;
	}
	if( internal_handle->file_descriptor == -1 )
	{
		internal_handle->file_descriptor = open(
		                                    filenames[ 0 ],
		                                    file_io_flags,
		                                    0644 );

		if( internal_handle->file_descriptor == -1 )
		{
			switch( errno )
			{
				case EACCES:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_ACCESS_DENIED,
					 "%s: access denied to device: %s.",
					 function,
					 filenames[ 0 ] );

					break;

				case ENOENT:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_INVALID_RESOURCE,
					 "%s: no such file: %s.",
					 function,
					 filenames[ 0 ] );

					break;

				default:
					libcerror_system_set_error(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 errno,
					 "%s: unable to open file: %s.",
					 function,
					 filenames[ 0 ] );

					break;
			}
			goto on_error;
		}
#if defined( HAVE_POSIX_FADVISE )
		/* Use this function to double the read-ahead system buffer
		 * This provides for some additional performance
		 */
		if( posix_fadvise(
		     internal_handle->file_descriptor,
		     0,
		     0,
		     POSIX_FADV_SEQUENTIAL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to advice file descriptor.",
			 function );

			goto on_error;
		}
#endif
	}
#endif
	if( libsmdev_handle_get_media_size(
	     handle,
	     &media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
#if defined( WINAPI )
	if( internal_handle->file_handle != INVALID_HANDLE_VALUE )
	{
		CloseHandle(
		 internal_handle->file_handle );

		internal_handle->file_handle = INVALID_HANDLE_VALUE;
	}
#else
	if( internal_handle->file_descriptor != -1 )
	{
		close(
		 internal_handle->file_descriptor );

		internal_handle->file_descriptor = -1;
	}
#endif
	if( internal_handle->filename != NULL )
	{
		memory_free(
		 internal_handle->filename );

		internal_handle->filename      = NULL;
		internal_handle->filename_size = 0;
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a storage media device
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_open_wide(
     libsmdev_handle_t *handle,
     wchar_t * const filenames[],
     int number_of_filenames,
     int access_flags,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_open_wide";
	size64_t media_size                         = 0;
	size_t filename_length                      = 0;

#if defined( WINAPI )
	DWORD error_code                            = 0;
	DWORD file_io_access_flags                  = 0;
	DWORD file_io_creation_flags                = 0;
#else
	char *narrow_filename                       = NULL;
	size_t narrow_filename_size                 = 0;
	int file_io_flags                           = 0;
	int result                                  = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - name already exists.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of filenames value out of bounds.",
		 function );

		return( -1 );
	}
	if( number_of_filenames != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: currently only one device file supported.",
		 function );

		return( -1 );
	}
	if( filenames[ 0 ] == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing device filename.",
		 function );

		return( -1 );
	}
	if( libsmdev_array_resize(
	     internal_handle->tracks_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_track_value_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty tracks array.",
		 function );

		goto on_error;
	}
	if( libsmdev_array_resize(
	     internal_handle->sessions_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty sessions array.",
		 function );

		goto on_error;
	}
	if( libsmdev_array_resize(
	     internal_handle->lead_outs_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty lead-outs array.",
		 function );

		goto on_error;
	}
	if( libsmdev_offset_list_empty(
	     internal_handle->errors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty errors offset list.",
		 function );

		goto on_error;
	}
	filename_length = libcstring_wide_string_length(
	                   filenames[ 0 ] );

	if( libsmdev_handle_set_filename_wide(
	     handle,
	     filenames[ 0 ],
	     filename_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in handle.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
	if( ( ( access_flags & LIBSMDEV_ACCESS_FLAG_READ ) != 0 )
	 && ( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 ) )
	{
		file_io_access_flags   = GENERIC_WRITE | GENERIC_READ;
		file_io_creation_flags = OPEN_ALWAYS;
	}
	else if( ( access_flags & LIBSMDEV_ACCESS_FLAG_READ ) != 0 )
	{
		file_io_access_flags   = GENERIC_READ;
		file_io_creation_flags = OPEN_EXISTING;
	}
	else if( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 )
	{
		file_io_access_flags   = GENERIC_WRITE;
		file_io_creation_flags = OPEN_ALWAYS;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		goto on_error;
	}
	if( ( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 )
	 && ( ( access_flags & LIBSMDEV_ACCESS_FLAG_TRUNCATE ) != 0 ) )
	{
		file_io_creation_flags = TRUNCATE_EXISTING;
	}
	if( internal_handle->file_handle == INVALID_HANDLE_VALUE )
	{
		/* Must use CreateFileW here because filename is a 
		 * wide character string and CreateFile is dependent
		 * on UNICODE directives
		 */
		internal_handle->file_handle = CreateFileW(
		                                (LPCWSTR) filenames[ 0 ],
		                                file_io_access_flags,
		                                0,
		                                NULL,
		                                file_io_creation_flags,
		                                FILE_ATTRIBUTE_NORMAL,
		                                NULL );

		if( internal_handle->file_handle == INVALID_HANDLE_VALUE )
		{
			error_code = GetLastError();

			switch( error_code )
			{
				case ERROR_ACCESS_DENIED:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_ACCESS_DENIED,
					 "%s: access denied to device: %ls.",
					 function,
					 filenames[ 0 ] );

					break;

				case ERROR_FILE_NOT_FOUND:
				case ERROR_PATH_NOT_FOUND:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_INVALID_RESOURCE,
					 "%s: no such file: %ls.",
					 function,
					 filenames[ 0 ] );

					break;

				default:
					libcerror_system_set_error(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 error_code,
					 "%s: unable to open file: %ls.",
					 function,
					 filenames[ 0 ] );

					break;
			}
			goto on_error;
		}
	}
#else
	if( ( ( access_flags & LIBSMDEV_ACCESS_FLAG_READ ) != 0 )
	 && ( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 ) )
	{
		file_io_flags = O_RDWR | O_CREAT;
	}
	else if( ( access_flags & LIBSMDEV_ACCESS_FLAG_READ ) != 0 )
	{
		file_io_flags = O_RDONLY;
	}
	else if( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 )
	{
		file_io_flags = O_WRONLY | O_CREAT;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		goto on_error;
	}
	if( ( ( access_flags & LIBSMDEV_ACCESS_FLAG_WRITE ) != 0 )
	 && ( ( access_flags & LIBSMDEV_ACCESS_FLAG_TRUNCATE ) != 0 ) )
	{
		file_io_flags |= O_TRUNC;
	}
	if( internal_handle->file_descriptor == -1 )
	{
		if( libclocale_codepage == 0 )
		{
#if SIZEOF_WCHAR_T == 4
			result = libuna_utf8_string_size_from_utf32(
				  (libuna_utf32_character_t *) filenames[ 0 ],
				  filename_length + 1,
				  &narrow_filename_size,
				  error );
#elif SIZEOF_WCHAR_T == 2
			result = libuna_utf8_string_size_from_utf16(
				  (libuna_utf16_character_t *) filenames[ 0 ],
				  filename_length + 1,
				  &narrow_filename_size,
				  error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
		}
		else
		{
#if SIZEOF_WCHAR_T == 4
			result = libuna_byte_stream_size_from_utf32(
				  (libuna_utf32_character_t *) filenames[ 0 ],
				  filename_length + 1,
				  libclocale_codepage,
				  &narrow_filename_size,
				  error );
#elif SIZEOF_WCHAR_T == 2
			result = libuna_byte_stream_size_from_utf16(
				  (libuna_utf16_character_t *) filenames[ 0 ],
				  filename_length + 1,
				  libclocale_codepage,
				  &narrow_filename_size,
				  error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to determine narrow character filename size.",
			 function );

			goto on_error;
		}
		narrow_filename = libcstring_narrow_string_allocate(
		                   narrow_filename_size );

		if( narrow_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create narrow character filename.",
			 function );

			goto on_error;
		}
		if( libclocale_codepage == 0 )
		{
#if SIZEOF_WCHAR_T == 4
			result = libuna_utf8_string_copy_from_utf32(
				  (libuna_utf8_character_t *) narrow_filename,
				  narrow_filename_size,
				  (libuna_utf32_character_t *) filenames[ 0 ],
				  filename_length + 1,
				  error );
#elif SIZEOF_WCHAR_T == 2
			result = libuna_utf8_string_copy_from_utf16(
				  (libuna_utf8_character_t *) narrow_filename,
				  narrow_filename_size,
				  (libuna_utf16_character_t *) filenames[ 0 ],
				  filename_length + 1,
				  error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
		}
		else
		{
#if SIZEOF_WCHAR_T == 4
			result = libuna_byte_stream_copy_from_utf32(
				  (uint8_t *) narrow_filename,
				  narrow_filename_size,
				  libclocale_codepage,
				  (libuna_utf32_character_t *) filenames[ 0 ],
				  filename_length + 1,
				  error );
#elif SIZEOF_WCHAR_T == 2
			result = libuna_byte_stream_copy_from_utf16(
				  (uint8_t *) narrow_filename,
				  narrow_filename_size,
				  libclocale_codepage,
				  (libuna_utf16_character_t *) filenames[ 0 ],
				  filename_length + 1,
				  error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set narrow character filename.",
			 function );

			memory_free(
			 narrow_filename );

			goto on_error;
		}
		internal_handle->file_descriptor = open(
		                                    narrow_filename,
		                                    file_io_flags,
		                                    0644 );

		memory_free(
		 narrow_filename );

		if( internal_handle->file_descriptor == -1 )
		{
			switch( errno )
			{
				case EACCES:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_ACCESS_DENIED,
					 "%s: access denied to device: %ls.",
					 function,
					 filenames[ 0 ] );

					break;

				case ENOENT:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_INVALID_RESOURCE,
					 "%s: no such file: %ls.",
					 function,
					 filenames[ 0 ] );

					break;

				default:
					libcerror_system_set_error(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 errno,
					 "%s: unable to open file: %ls.",
					 function,
					 filenames[ 0 ] );

					break;
			}
			goto on_error;
		}
#if defined( HAVE_POSIX_FADVISE )
		/* Use this function to double the read-ahead system buffer
		 * This provides for some additional performance
		 */
		if( posix_fadvise(
		     internal_handle->file_descriptor,
		     0,
		     0,
		     POSIX_FADV_SEQUENTIAL ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to advice file descriptor.",
			 function );

			goto on_error;
		}
#endif
	}
#endif
	if( libsmdev_handle_get_media_size(
	     handle,
	     &media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
#if defined( WINAPI )
	if( internal_handle->file_handle != INVALID_HANDLE_VALUE )
	{
		CloseHandle(
		 internal_handle->file_handle );

		internal_handle->file_handle = INVALID_HANDLE_VALUE;
	}
#else
	if( internal_handle->file_descriptor != -1 )
	{
		close(
		 internal_handle->file_descriptor );

		internal_handle->file_descriptor = -1;
	}
#endif
	if( internal_handle->filename != NULL )
	{
		memory_free(
		 internal_handle->filename );

		internal_handle->filename      = NULL;
		internal_handle->filename_size = 0;
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Closes a device handle
 * Returns the 0 if succesful or -1 on error
 */
int libsmdev_handle_close(
     libsmdev_handle_t *handle,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_close";
	int result                                  = 0;

#if defined( WINAPI )
	DWORD error_code                            = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

#if defined( WINAPI )
	if( internal_handle->file_handle == INVALID_HANDLE_VALUE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file handle.",
		 function );

		return( -1 );
	}
	if( CloseHandle(
	     internal_handle->file_handle ) == 0 )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 error_code,
		 "%s: unable to close handle.",
		 function );

		result = -1;
	}
	internal_handle->file_handle = INVALID_HANDLE_VALUE;
#else
	if( internal_handle->file_descriptor == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file descriptor.",
		 function );

		return( -1 );
	}
	if( close(
	     internal_handle->file_descriptor ) != 0 )
	{
		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 errno,
		 "%s: unable to close file descriptor.",
		 function );

		result = -1;
	}
	internal_handle->file_descriptor = -1;
#endif
	if( libsmdev_array_resize(
	     internal_handle->tracks_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_track_value_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty tracks array.",
		 function );

		result = -1;
	}
	if( libsmdev_array_resize(
	     internal_handle->sessions_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty sessions array.",
		 function );

		result = -1;
	}
	if( libsmdev_array_resize(
	     internal_handle->lead_outs_array,
	     0,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libsmdev_sector_range_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty lead-outs array.",
		 function );

		result = -1;
	}
	if( libsmdev_offset_list_empty(
	     internal_handle->errors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty errors offset list.",
		 function );

		result = -1;
	}
	return( result );
}

/* Reads a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libsmdev_handle_read_buffer(
         libsmdev_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
#if defined( HAVE_VERBOSE_OUTPUT )
	libcstring_system_character_t error_string[ LIBSMDEV_ERROR_STRING_DEFAULT_SIZE ];
#endif

	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_read_buffer";
	off64_t calculated_current_offset           = 0;
	off64_t current_offset                      = 0;
	size_t buffer_offset                        = 0;
	size_t error_granularity_buffer_offset      = 0;
	size_t error_granularity_size               = 0;
	size_t error_granularity_skip_size          = 0;
	size_t read_error_size                      = 0;
	size_t read_size                            = 0;
	ssize_t read_count                          = 0;
	int16_t number_of_read_errors               = 0;

#if defined( WINAPI )
	LARGE_INTEGER large_integer_offset          = LIBSMDEV_LARGE_INTEGER_ZERO;
	DWORD error_code                            = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing filename.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( internal_handle->file_handle == INVALID_HANDLE_VALUE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file handle.",
		 function );

		return( -1 );
	}
#else
	if( internal_handle->file_descriptor == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file descriptor.",
		 function );

		return( -1 );
	}
#endif
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
#if UINT_MAX < SIZE_MAX
	if( buffer_size > (size_t) UINT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
#else
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
	read_size = buffer_size;

	if( internal_handle->media_size != 0 )
	{
		if( internal_handle->offset >= (off64_t) internal_handle->media_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: offset exceeds media size.",
			 function );

			return( -1 );
		}
		if( ( internal_handle->offset + (off64_t) read_size ) > (off64_t) internal_handle->media_size )
		{
			read_size = (size_t) ( internal_handle->media_size - internal_handle->offset );
		}
	}
	while( number_of_read_errors <= (int16_t) internal_handle->number_of_error_retries )
	{
		if( internal_handle->abort != 0 )
		{
			break;
		}
		if( read_size == 0 )
		{
			break;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading buffer at offset: %" PRIi64 " of size: %" PRIzd ".\n",
			 function,
			 internal_handle->offset + (off64_t) buffer_offset,
			 read_size );
		}
#endif
#if defined( WINAPI )
		error_code = 0;

		if( ReadFile(
		     internal_handle->file_handle,
		     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
		     read_size,
		     (LPDWORD) &read_count,
		     NULL ) == 0 )
		{
			/* TODO improve error handling */

			error_code = GetLastError();

			switch( error_code )
			{
				case ERROR_HANDLE_EOF:
					break;

				/* A read error generates this error code
				 */
				case ERROR_UNRECOGNIZED_MEDIA:
					break;

				default:
					libcerror_system_set_error(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 error_code,
					 "%s: unable to read from device.",
					 function );

					return( -1 );
			}
		}
		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: invalid read count: %" PRIzd " returned.",
			 function,
			 read_count );

			return( -1 );
		}
		if( error_code == ERROR_HANDLE_EOF )
		{
			return( 0 );
		}
		else if( error_code == ERROR_UNRECOGNIZED_MEDIA )
		{
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x520
			large_integer_offset.QuadPart = (LONGLONG) 0;
#else
			large_integer_offset.LowPart  = (DWORD) 0;
			large_integer_offset.HighPart = (LONG) 0;
#endif

#if ( WINVER >= 0x0500 )
			if( SetFilePointerEx(
			     internal_handle->file_handle,
			     large_integer_offset,
			     &large_integer_offset,
			     FILE_CURRENT ) == 0 )
#else
			if( libsmdev_SetFilePointerEx(
			     internal_handle->file_handle,
			     large_integer_offset,
			     &large_integer_offset,
			     FILE_CURRENT ) == 0 )
#endif
			{
				error_code = GetLastError();

				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 error_code,
				 "%s: unable to seek current offset.",
				 function );

				return( -1 );
			}
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x520
			current_offset = (off64_t) large_integer_offset.QuadPart;
#else
			current_offset = ( (off64_t) large_integer_offset.HighPart << 32 ) + large_integer_offset.LowPart;
#endif
			calculated_current_offset = internal_handle->offset + (off64_t) buffer_offset;

			if( current_offset != calculated_current_offset )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: correcting offset drift (actual: %" PRIi64 ", calculated: %" PRIi64 ").\n",
					 function,
					 current_offset,
					 calculated_current_offset );
				}
#endif
				if( current_offset < calculated_current_offset )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: unable to to correct negative offset drift.",
					 function );

					return( -1 );
				}
				read_count = (ssize_t) ( current_offset - calculated_current_offset );
			}
		}
		if( read_count > (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid read count value exceeds read size.",
			 function );

			return( -1 );
		}
		buffer_offset += read_count;
		read_size     -= read_count;

		if( read_size == 0 )
		{
			break;
		}
#else
		read_count = read(
			      internal_handle->file_descriptor,
			      &( ( (uint8_t *) buffer )[ buffer_offset ] ),
			      read_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: read buffer at offset: %" PRIi64 " of size: %" PRIzd ".\n",
			 function,
			 internal_handle->offset + (off64_t) buffer_offset,
			 read_count );
		}
#endif
		if( read_count < 0 )
		{
			switch( errno )
			{
				/* Reading should not be retries for some error conditions
				 */
				case ESPIPE:
				case EPERM:
				case ENXIO:
				case ENODEV:
					libcerror_system_set_error(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 errno,
					 "%s: unable to read from device.",
					 function );

					return( -1 );

				default:
#if defined( HAVE_VERBOSE_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( libsmdev_error_string_copy_from_error_number(
						     error_string,
						     LIBSMDEV_ERROR_STRING_DEFAULT_SIZE,
						     errno,
						     error ) != 0 )
						{
							libcnotify_printf(
							 "%s: unable to read from device with error: %" PRIs_LIBCSTRING_SYSTEM "\n",
							 function,
							 error_string );
						}
						else
						{
							libcnotify_printf(
							 "%s: unable to read from device.\n",
							 function );
						}
					}
#endif
					break;
			}
			current_offset = lseek(
			                  internal_handle->file_descriptor,
			                  0,
			                  SEEK_CUR );

			if( current_offset < 0 )
			{
				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 errno,
				 "%s: unable to seek current offset.",
				 function );

				return( -1 );
			}
			calculated_current_offset = internal_handle->offset + (off64_t) buffer_offset;

			/* On MacOS-X the read count is -1 on error and the file offset is set to the position of the error
			 */
			if( current_offset != calculated_current_offset )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: correcting offset drift (actual: %" PRIi64 ", calculated: %" PRIi64 ").\n",
					 function,
					 current_offset,
					 calculated_current_offset );
				}
#endif
				if( current_offset < calculated_current_offset )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: unable to to correct negative offset drift.",
					 function );

					return( -1 );
				}
				read_count = (ssize_t) ( current_offset - calculated_current_offset );
			}
		}
		if( read_count > (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid read count value exceeds read size.",
			 function );

			return( -1 );
		}
		if( read_count == 0 )
		{
			return( 0 );
		}
		else if( read_count > 0 )
		{
			buffer_offset += read_count;
			read_size     -= read_count;

			if( read_size == 0 )
			{
				break;
			}
		}
#endif /* defined( WINAPI ) */
		/* Not all data have been read or there was an error
		 */
		number_of_read_errors++;

#if defined( HAVE_VERBOSE_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: read error: %" PRIi16 " at offset %" PRIi64 ".\n",
			 function,
			 number_of_read_errors,
			 internal_handle->offset + buffer_offset );
		}
#endif
		if( number_of_read_errors > (int16_t) internal_handle->number_of_error_retries )
		{
			if( internal_handle->error_granularity > 0 )
			{
				error_granularity_size = internal_handle->error_granularity;
			}
			else
			{
				error_granularity_size = buffer_size;
			}
			error_granularity_buffer_offset = ( buffer_offset / error_granularity_size ) * error_granularity_size;
			error_granularity_skip_size     = ( error_granularity_buffer_offset + error_granularity_size ) - buffer_offset;

			/* Check if error granularity skip is still within range of the buffer
			 */
			if( error_granularity_skip_size > read_size )
			{
				error_granularity_skip_size = read_size;
			}
			if( ( internal_handle->error_flags & LIBSMDEV_ERROR_FLAG_ZERO_ON_ERROR ) != 0 )
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: zero-ing buffer of size: %" PRIzd " bytes at offset %" PRIzd ".\n",
					 function,
					 error_granularity_size,
					 error_granularity_buffer_offset );
				}
#endif

				if( memory_set(
				     &( ( (uint8_t *) buffer )[ error_granularity_buffer_offset ] ),
				     0,
				     error_granularity_size ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to zero buffer on error.",
					 function );

					return( -1 );
				}
				read_error_size = error_granularity_size;
			}
			else
			{
#if defined( HAVE_VERBOSE_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: zero-ing remainder of buffer of size: %" PRIzd " bytes at offset %" PRIzd ".\n",
					 function,
					 error_granularity_skip_size,
					 buffer_offset );
				}
#endif

				if( memory_set(
				     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
				     0,
				     error_granularity_skip_size ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to zero remainder of buffer on error.",
					 function );

					return( -1 );
				}
				read_error_size = error_granularity_skip_size;
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: adding read error at offset: %" PRIi64 ", number of bytes: %" PRIzd ".\n",
				 function,
				 current_offset,
				 read_error_size );
			}
#endif
			if( libsmdev_offset_list_append_offset(
			     internal_handle->errors,
			     current_offset,
			     read_error_size,
			     1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append read error to offset list.",
				 function );

				return( -1 );
			}
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: skipping %" PRIu32 " bytes.\n",
				 function,
				 error_granularity_skip_size );
			}
#endif

#if defined( WINAPI )
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x520
			large_integer_offset.QuadPart = (LONGLONG) error_granularity_skip_size;
#else
			large_integer_offset.LowPart  = (DWORD) error_granularity_skip_size;
			large_integer_offset.HighPart = (LONG) 0;
#endif

#if ( WINVER >= 0x0500 )
			if( SetFilePointerEx(
			     internal_handle->file_handle,
			     large_integer_offset,
			     &large_integer_offset,
			     FILE_CURRENT ) == 0 )
#else
			if( libsmdev_SetFilePointerEx(
			     internal_handle->file_handle,
			     large_integer_offset,
			     &large_integer_offset,
			     FILE_CURRENT ) == 0 )
#endif
			{
				error_code = GetLastError();

				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 error_code,
				 "%s: unable skip %" PRIu32 " bytes after sector.",
				 function,
				 error_granularity_skip_size );

				return( -1 );
			}
#else
			if( lseek(
			     internal_handle->file_descriptor,
			     error_granularity_skip_size,
			     SEEK_CUR ) == -1 )
			{
				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 errno,
				 "%s: unable to skip %" PRIu32 " bytes after sector.",
				 function,
				 error_granularity_skip_size );

				return( -1 );
			}
#endif /* defined( WINAPI ) */
			read_size            -= error_granularity_skip_size;
			buffer_offset        += error_granularity_skip_size;
			number_of_read_errors = 0;
		}
	}
	internal_handle->offset += buffer_offset;

	return( (ssize_t) buffer_offset );
}

/* Writes a buffer
 * Returns the number of bytes written or -1 on error
 */
ssize_t libsmdev_handle_write_buffer(
         libsmdev_handle_t *handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_write_buffer";
	ssize_t write_count                         = 0;

#if defined( WINAPI )
	DWORD error_code                            = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing filename.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( internal_handle->file_handle == INVALID_HANDLE_VALUE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file handle.",
		 function );

		return( -1 );
	}
#else
	if( internal_handle->file_descriptor == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file descriptor.",
		 function );

		return( -1 );
	}
#endif
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
#if UINT_MAX < SIZE_MAX
	if( buffer_size > (size_t) UINT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
#endif
	if( WriteFile(
	     internal_handle->file_handle,
	     buffer,
	     buffer_size,
	     (LPDWORD) &write_count,
	     NULL ) == 0 )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 error_code,
		 "%s: unable to write to device.",
		 function );

		return( -1 );
	}
	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: invalid write count: %" PRIzd " returned.",
		 function,
		 write_count );

		return( -1 );
	}
#else
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	write_count = write(
	               internal_handle->file_descriptor,
	               buffer,
	               buffer_size );

	if( write_count < 0 )
	{
		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 errno,
		 "%s: unable to write to device.",
		 function );

		return( -1 );
	}
#endif
	internal_handle->offset += write_count;

	return( write_count );
}

#if defined( WINAPI ) && ( WINVER < 0x0500 )

#if !defined( INVALID_SET_FILE_POINTER )
#define INVALID_SET_FILE_POINTER	((LONG) -1)
#endif

/* Cross Windows safe version of SetFilePointerEx
 * Returns TRUE if successful or FALSE on error
 */
BOOL libsmdev_SetFilePointerEx(
      HANDLE file_handle,
      LARGE_INTEGER distance_to_move_large_integer,
      LARGE_INTEGER *new_file_pointer_large_integer,
      DWORD move_method )
{
	FARPROC function                 = NULL;
	HMODULE library_handle           = NULL;
	LONG distance_to_move_lower_long = 0;
	LONG distance_to_move_upper_long = 0;
	DWORD error_number               = 0;
	BOOL result                      = FALSE;

	if( file_handle == NULL )
	{
		return( FALSE );
	}
	if( new_file_pointer_large_integer == NULL )
	{
		return( FALSE );
	}
	library_handle = LoadLibrary(
	                  _LIBCSTRING_SYSTEM_STRING( "kernel32.dll" ) );

	if( library_handle == NULL )
	{
		return( FALSE );
	}
	function = GetProcAddress(
		    library_handle,
		    (LPCSTR) "SetFilePointerEx" );

	if( function != NULL )
	{
		result = function(
			  file_handle,
			  distance_to_move_large_integer,
			  new_file_pointer_large_integer,
			  move_method );
	}
	else
	{
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x520
		distance_to_move_lower_long = distance_to_move_large_integer.QuadPart & 0xffffffffUL;
		distance_to_move_upper_long = distance_to_move_large_integer.QuadPart >> 32;
#else
		distance_to_move_lower_long = distance_to_move_large_integer.LowPart;
		distance_to_move_upper_long = distance_to_move_large_integer.HighPart;
#endif

		distance_to_move_lower_long = SetFilePointer(
					       file_handle,
					       distance_to_move_lower_long,
					       &distance_to_move_upper_long,
					       move_method );

		error_number = GetLastError();

		if( ( distance_to_move_lower_long == (LONG) INVALID_SET_FILE_POINTER )
		 && ( error_number != NO_ERROR ) )
		{
		}
		else
		{
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x520
			new_file_pointer_large_integer->QuadPart   = distance_to_move_upper_long;
			new_file_pointer_large_integer->QuadPart <<= 32;
			new_file_pointer_large_integer->QuadPart  += distance_to_move_lower_long;
#else
			new_file_pointer_large_integer->HighPart = distance_to_move_upper_long;
			new_file_pointer_large_integer->LowPart  = distance_to_move_lower_long;
#endif

			result = TRUE;
		}
	}
	/* This call should be after using the function
	 * in most cases kernel32.dll will still be available after free
	 */
	if( FreeLibrary(
	     library_handle ) != TRUE )
	{
		result = FALSE;
	}
	return( result );
}

#endif

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libsmdev_handle_seek_offset(
         libsmdev_handle_t *handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_seek_offset";

#if defined( WINAPI )
	LARGE_INTEGER large_integer_offset          = LIBSMDEV_LARGE_INTEGER_ZERO;
	DWORD error_code                            = 0;
	DWORD move_method                           = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing filename.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( internal_handle->file_handle == INVALID_HANDLE_VALUE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file handle.",
		 function );

		return( -1 );
	}
#else
	if( internal_handle->file_descriptor == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing file descriptor.",
		 function );

		return( -1 );
	}
#endif
	if( offset > (off64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( whence == SEEK_SET )
	{
		move_method = FILE_BEGIN;
	}
	else if( whence == SEEK_CUR )
	{
		move_method = FILE_CURRENT;
	}
	else if( whence == SEEK_END )
	{
		move_method = FILE_END;
	}
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x520
	large_integer_offset.QuadPart = (LONGLONG) offset;
#else
	large_integer_offset.LowPart  = (DWORD) ( 0x0ffffffffUL & offset );
	large_integer_offset.HighPart = (LONG) ( offset >> 32 );
#endif

#if ( WINVER >= 0x0500 )
	if( SetFilePointerEx(
	     internal_handle->file_handle,
	     large_integer_offset,
	     &large_integer_offset,
	     move_method ) == 0 )
#else
	if( libsmdev_SetFilePointerEx(
	     internal_handle->file_handle,
	     large_integer_offset,
	     &large_integer_offset,
	     move_method ) == 0 )
#endif
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 error_code,
		 "%s: unable to find offset: %" PRIi64 " in device.",
		 function,
		 offset );

		return( -1 );
	}
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x520
	offset = (off64_t) large_integer_offset.QuadPart;
#else
	offset = ( (off64_t) large_integer_offset.HighPart << 32 ) + large_integer_offset.LowPart;
#endif

	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: invalid offset: %" PRIi64 " returned.",
		 function,
		 offset );

		return( -1 );
	}
#else
	offset = lseek(
	          internal_handle->file_descriptor,
	          (off_t) offset,
	          whence );

	if( offset < 0 )
	{
		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 errno,
		 "%s: unable to find offset: %" PRIi64 " in device.",
		 function,
		 offset );

		return( -1 );
	}
#endif
	internal_handle->offset = offset;

	return( offset );
}

/* Retrieves the current offset of the (media) data
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_offset(
     libsmdev_handle_t *handle,
     off64_t *offset,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_offset";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	*offset = internal_handle->offset;

	return( 1 );
}

/* Retrieves the filename size of the file handle
 * The filename size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmdev_handle_get_filename_size(
     libsmdev_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_filename_size";

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing filename.",
		 function );

		return( -1 );
	}
	if( filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename size.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#else
	*filename_size = internal_handle->filename_size;
#endif
	return( 1 );
}

/* Retrieves the filename of the file handle
 * The filename size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmdev_handle_get_filename(
     libsmdev_handle_t *handle,
     char *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_filename";
	size_t narrow_filename_size                 = 0;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing filename.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          &narrow_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          &narrow_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          &narrow_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          &narrow_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#else
	narrow_filename_size = internal_handle->filename_size;
#endif
	if( filename_size < narrow_filename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: filename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) filename,
		          filename_size,
		          (libuna_utf32_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) filename,
		          filename_size,
		          (libuna_utf16_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) filename,
		          filename_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) filename,
		          filename_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set filename.",
		 function );

		return( -1 );
	}
#else
	if( libcstring_system_string_copy(
	     filename,
	     internal_handle->filename,
	     internal_handle->filename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set filename.",
		 function );

		return( -1 );
	}
	filename[ internal_handle->filename_size - 1 ] = 0;
#endif
	return( 1 );
}

/* Sets the filename for the file handle
 * Returns 1 if succesful or -1 on error
 */
int libsmdev_handle_set_filename(
     libsmdev_handle_t *handle,
     const char *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                      = "libsmdev_handle_set_filename";

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                 = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid filename length is zero.",
		 function );

		return( -1 );
	}
	if( filename_length >= (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid filename length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->filename != NULL )
	{
#if defined( WINAPI )
		if( internal_handle->file_handle != INVALID_HANDLE_VALUE )
#else
		if( internal_handle->file_descriptor != -1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: filename already set: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 internal_handle->filename );

			return( -1 );
		}
		memory_free(
		 internal_handle->filename );

		internal_handle->filename      = NULL;
		internal_handle->filename_size = 0;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) filename,
		          filename_length + 1,
		          &( internal_handle->filename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) filename,
		          filename_length + 1,
		          &( internal_handle->filename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &( internal_handle->filename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &( internal_handle->filename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#else
	internal_handle->filename_size = filename_length + 1;
#endif
	internal_handle->filename = libcstring_system_string_allocate(
	                             internal_handle->filename_size );

	if( internal_handle->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create filename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          (libuna_utf8_character_t *) filename,
		          filename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          (libuna_utf8_character_t *) filename,
		          filename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          (uint8_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          (uint8_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set filename.",
		 function );

		memory_free(
		 internal_handle->filename );

		internal_handle->filename      = NULL;
		internal_handle->filename_size = 0;

		return( -1 );
	}
#else
	if( libcstring_system_string_copy(
	     internal_handle->filename,
	     filename,
	     filename_length + 1 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set filename.",
		 function );

		memory_free(
		 internal_handle->filename );

		internal_handle->filename      = NULL;
		internal_handle->filename_size = 0;

		return( -1 );
	}
	internal_handle->filename[ filename_length ] = 0;
#endif
	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves the filename size of the file handle
 * The filename size includes the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmdev_handle_get_filename_size_wide(
     libsmdev_handle_t *handle,
     size_t *filename_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_filename_size_wide";

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing filename.",
		 function );

		return( -1 );
	}
	if( filename_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename size.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	*filename_size = internal_handle->filename_size;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the filename of the file handle
 * The filename size should include the end of string character
 * Returns 1 if succesful or -1 on error
 */
int libsmdev_handle_get_filename_wide(
     libsmdev_handle_t *handle,
     wchar_t *filename,
     size_t filename_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_filename_wide";
	size_t wide_filename_size                   = 0;

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( internal_handle->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing filename.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	wide_filename_size = internal_handle->filename_size;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          &wide_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          &wide_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          &wide_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          &wide_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#endif
	if( filename_size < wide_filename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: filename too small.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_system_string_copy(
	     filename,
	     internal_handle->filename,
	     internal_handle->filename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set filename.",
		 function );

		return( -1 );
	}
	filename[ internal_handle->filename_size - 1 ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) filename,
		          filename_size,
		          (libuna_utf8_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) filename,
		          filename_size,
		          (libuna_utf8_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) filename,
		          filename_size,
		          (uint8_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) filename,
		          filename_size,
		          (uint8_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set filename.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the filename for the file handle
 * Returns 1 if succesful or -1 on error
 */
int libsmdev_handle_set_filename_wide(
     libsmdev_handle_t *handle,
     const wchar_t *filename,
     size_t filename_length,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_set_filename_wide";

#if !defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	int result                                  = 0;
#endif

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	internal_handle = (libsmdev_internal_handle_t *) handle;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid filename length is zero.",
		 function );

		return( -1 );
	}
	if( filename_length >= (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid filename length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( internal_handle->filename != NULL )
	{
#if defined( WINAPI )
		if( internal_handle->file_handle != INVALID_HANDLE_VALUE )
#else
		if( internal_handle->file_descriptor != -1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: filename already set: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 internal_handle->filename );

			return( -1 );
		}
		memory_free(
		 internal_handle->filename );

		 internal_handle->filename      = NULL;
		 internal_handle->filename_size = 0;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	internal_handle->filename_size = filename_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          &( internal_handle->filename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          &( internal_handle->filename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &( internal_handle->filename_size ),
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &( internal_handle->filename_size ),
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine filename size.",
		 function );

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	internal_handle->filename = libcstring_system_string_allocate(
	                             internal_handle->filename_size );

	if( internal_handle->filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create filename.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcstring_system_string_copy(
	     internal_handle->filename,
	     filename,
	     filename_length + 1 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set filename.",
		 function );

		memory_free(
		 internal_handle->filename );

		internal_handle->filename      = NULL;
		internal_handle->filename_size = 0;

		return( -1 );
	}
	internal_handle->filename[ filename_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) internal_handle->filename,
		          internal_handle->filename_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set filename.",
		 function );

		memory_free(
		 internal_handle->filename );

		internal_handle->filename      = NULL;
		internal_handle->filename_size = 0;

		return( -1 );
	}
#endif /* defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Function to determine if a file exists
 * Returns 1 if file exists, 0 if not or -1 on error
 */
int libsmdev_file_exists(
     const char *filename,
     libcerror_error_t **error )
{
	static char *function = "libsmdev_file_exists";
	int result            = 1;

#if defined( WINAPI )
	HANDLE file_handle    = INVALID_HANDLE_VALUE;
	DWORD error_code      = 0;
#else
	int file_descriptor   = -1;
#endif

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	/* Must use CreateFileA here because filename is a 
	 * narrow character string and CreateFile is dependent
	 * on UNICODE directives
	 */
	file_handle = CreateFileA(
	               (LPCSTR) filename,
	               GENERIC_READ,
	               FILE_SHARE_READ,
	               NULL,
	               OPEN_EXISTING,
	               FILE_ATTRIBUTE_NORMAL,
	               NULL );

	if( file_handle == INVALID_HANDLE_VALUE )
	{
		error_code = GetLastError();

		switch( error_code )
		{
			case ERROR_ACCESS_DENIED:
				result = 1;

				break;

			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
				result = 0;

				break;

			default:
				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 error_code,
				 "%s: unable to open file: %s.",
				 function,
				 filename );

				result = -1;

				break;
		}
	}
	else if( CloseHandle(
	          file_handle ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file: %s.",
		 function,
		 filename );

		return( -1 );
	}
#else
	file_descriptor = open(
	                   filename,
	                   O_RDONLY,
	                   0644 );

	if( file_descriptor == -1 )
	{
		switch( errno )
		{
			case EACCES:
				result = 1;

				break;

			case ENOENT:
				result = 0;

				break;

			default:
				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 errno,
				 "%s: unable to open file: %s.",
				 function,
				 filename );

				result = -1;

				break;
		}
	}
	else if( close(
		  file_descriptor ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file: %s.",
		 function,
		 filename );

		return( -1 );
	}
#endif
	return( result );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Function to determine if a file exists
 * Returns 1 if file exists, 0 if not or -1 on error
 */
int libsmdev_file_exists_wide(
     const wchar_t *filename,
     libcerror_error_t **error )
{
	static char *function       = "libsmdev_file_exists_wide";
	int result                  = 1;

#if defined( WINAPI )
	HANDLE file_handle          = INVALID_HANDLE_VALUE;
	DWORD error_code            = 0;
#else
	char *narrow_filename       = NULL;
	size_t filename_length      = 0;
	size_t narrow_filename_size = 0;
	int file_descriptor         = -1;
#endif

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	/* Must use CreateFileW here because filename is a 
	 * wide character string and CreateFile is dependent
	 * on UNICODE directives
	 */
	file_handle = CreateFileW(
	               (LPCWSTR) filename,
	               GENERIC_READ,
	               FILE_SHARE_READ,
	               NULL,
	               OPEN_EXISTING,
	               FILE_ATTRIBUTE_NORMAL,
	               NULL );

	if( file_handle == INVALID_HANDLE_VALUE )
	{
		error_code = GetLastError();

		switch( error_code )
		{
			case ERROR_ACCESS_DENIED:
				result = 1;

				break;

			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
				result = 0;

				break;

			default:
				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 error_code,
				 "%s: unable to open file: %ls.",
				 function,
				 filename );

				result = -1;

				break;
		}
	}
	else if( CloseHandle(
	          file_handle ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file: %ls.",
		 function,
		 filename );

		return( -1 );
	}
#else
	filename_length = libcstring_wide_string_length(
	                   filename );

	/* Convert the filename to a narrow string
	 * if the platform has no wide character open function
	 */
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          &narrow_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          &narrow_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &narrow_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          libclocale_codepage,
		          &narrow_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow character filename size.",
		 function );

		return( -1 );
	}
	narrow_filename = libcstring_narrow_string_allocate(
	                   narrow_filename_size );

	if( narrow_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create narrow character filename.",
		 function );

		return( -1 );
	}
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) narrow_filename,
		          narrow_filename_size,
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) narrow_filename,
		          narrow_filename_size,
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) narrow_filename,
		          narrow_filename_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) narrow_filename,
		          narrow_filename_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow character filename.",
		 function );

		memory_free(
		 narrow_filename );

		return( -1 );
	}
	file_descriptor = open(
	                   narrow_filename,
	                   O_RDONLY,
	                   0644 );

	memory_free(
	 narrow_filename );

	if( file_descriptor == -1 )
	{
		switch( errno )
		{
			case EACCES:
				result = 1;

				break;

			case ENOENT:
				result = 0;

				break;

			default:
				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_OPEN_FAILED,
				 errno,
				 "%s: unable to open file: %ls.",
				 function,
				 filename );

				result = -1;

				break;
		}
	}
	else if( close(
		  file_descriptor ) != 0 )
	{
		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 errno,
		 "%s: unable to close file descriptor.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

