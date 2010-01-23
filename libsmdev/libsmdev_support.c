/*
 * libsmdev support functions
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

#include <common.h>
#include <narrow_string.h>
#include <memory.h>
#include <wide_string.h>
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

#include "libsmdev_codepage.h"
#include "libsmdev_definitions.h"
#include "libsmdev_error_string.h"
#include "libsmdev_support.h"
#include "libsmdev_system_string.h"

/* Returns the library version
 */
const char *libsmdev_get_version(
             void )
{
	return( (const char *) LIBSMDEV_VERSION_STRING );
}

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libsmdev_get_codepage(
     int *codepage,
     liberror_error_t **error )
{
	static char *function = "libsmdev_get_codepage";

	if( codepage == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid codepage.",
		 function );

		return( -1 );
	}
	*codepage = libsmdev_system_narrow_string_codepage;

	return( 1 );
}

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libsmdev_set_codepage(
     int codepage,
     liberror_error_t **error )
{
	static char *function = "libsmdev_set_codepage";

	if( ( codepage != LIBSMDEV_CODEPAGE_ASCII )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_1 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_2 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_3 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_4 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_5 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_6 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_7 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_8 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_9 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_10 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_11 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_13 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_14 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_15 )
	 && ( codepage != LIBSMDEV_CODEPAGE_ISO_8859_16 )
	 && ( codepage != LIBSMDEV_CODEPAGE_WINDOWS_874 )
	 && ( codepage != LIBSMDEV_CODEPAGE_WINDOWS_1250 )
	 && ( codepage != LIBSMDEV_CODEPAGE_WINDOWS_1251 )
	 && ( codepage != LIBSMDEV_CODEPAGE_WINDOWS_1252 )
	 && ( codepage != LIBSMDEV_CODEPAGE_WINDOWS_1253 )
	 && ( codepage != LIBSMDEV_CODEPAGE_WINDOWS_1254 )
	 && ( codepage != LIBSMDEV_CODEPAGE_WINDOWS_1256 )
	 && ( codepage != LIBSMDEV_CODEPAGE_WINDOWS_1257 )
	 && ( codepage != LIBSMDEV_CODEPAGE_WINDOWS_1258 )
	 && ( codepage != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported codepage.",
		 function );

		return( -1 );
	}
	libsmdev_system_narrow_string_codepage = codepage;

	return( 1 );
}

/* Determines if a file is a device
 * Returns 1 if true, 0 if not or -1 on error
 */
int libsmdev_check_device(
     const char *filename,
     liberror_error_t **error )
{
	libsmdev_system_character_t error_string[ LIBSMDEV_ERROR_STRING_DEFAULT_SIZE ];

#if defined( WINAPI )
	HANDLE file_handle    = NULL;
	DWORD error_code      = 0;
	DWORD file_type       = 0;
#else
	struct stat file_stat;

	int file_descriptor   = 0;
#endif
	static char *function = "libsmdev_check_device";
	int result            = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_ACCESS_DENIED,
				 "%s: access denied to file: %s.",
				 function,
				 filename );

				break;

			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_INVALID_RESOURCE,
				 "%s: no such file: %s.",
				 function,
				 filename );

				break;

			default:
				if( libsmdev_error_string_copy_from_error_number(
				     error_string,
				     LIBSMDEV_ERROR_STRING_DEFAULT_SIZE,
				     error_code,
				     error ) != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open file: %s with error: %" PRIs_LIBSMDEV_SYSTEM "",
					 function,
					 filename,
					 error_string );
				}
				else
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open file: %s.",
					 function,
					 filename );
				}
				break;
		}
		return( -1 );
	}
	/* Use the GetFileType function to rule out certain file types
	 * like pipes, sockets, etc.
	 */
	file_type = GetFileType(
	             file_handle );

	if( file_type != FILE_TYPE_DISK )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		CloseHandle(
		 file_handle );

		return( -1 );
	}
	if( ( filename[ 0 ] == '\\' )
	 && ( filename[ 1 ] == '\\' )
	 && ( filename[ 2 ] == '.' )
	 && ( filename[ 3 ] == '\\' ) )
	{
		result = 1;
	}
	if( CloseHandle(
	     file_handle ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file: %s.",
		 function,
		 filename );

		return( -1 );
	}
#else
#if defined( HAVE_GLIB_H )
	file_descriptor = g_open(
	                   filename,
	                   file_io_flags,
	                   0644 );
#else
	file_descriptor = open(
	                   filename,
	                   O_RDONLY,
	                   0644 );

#endif /* HAVE_GLIB_H */

	if( file_descriptor == -1 )
	{
		switch( errno )
		{
			case EACCES:
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_ACCESS_DENIED,
				 "%s: access denied to file: %s.",
				 function,
				 filename );

				break;

			case ENOENT:
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_INVALID_RESOURCE,
				 "%s: no such file: %s.",
				 function,
				 filename );

				break;

			default:
				if( libsmdev_error_string_copy_from_error_number(
				     error_string,
				     LIBSMDEV_ERROR_STRING_DEFAULT_SIZE,
				     errno,
				     error ) != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open file: %s with error: %" PRIs_LIBSMDEV_SYSTEM "",
					 function,
					 filename,
					 error_string );
				}
				else
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open file: %s.",
					 function,
					 filename );
				}
				break;
		}
		return( -1 );
	}
	if( fstat(
	     file_descriptor,
	     &file_stat ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine file status information.",
		 function );

		close(
		 file_descriptor );

		return( -1 );
	}
	if( S_ISBLK( file_stat.st_mode )
	 || S_ISCHR( file_stat.st_mode ) )
	{
		result = 1;
	}
	if( close(
	     file_descriptor ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file: %s.",
		 function,
		 filename );

		return( -1 );
	}
#endif
	return( result );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file is a device
 * Returns 1 if true, 0 if not or -1 on error
 */
int libsmdev_check_device_wide(
     const wchar_t *filename,
     liberror_error_t **error )
{
	libsmdev_system_character_t error_string[ LIBSMDEV_ERROR_STRING_DEFAULT_SIZE ];

#if defined( WINAPI )
	HANDLE file_handle          = NULL;
	DWORD error_code            = 0;
	DWORD file_type             = 0;
#else
	struct stat file_stat;

        char *narrow_filename       = NULL;
        size_t narrow_filename_size = 0;
        size_t filename_length      = 0;

	int file_descriptor         = 0;
#endif
	static char *function       = "libsmdev_check_device_wide";
	int result                  = 0;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_ACCESS_DENIED,
				 "%s: access denied to file: %ls.",
				 function,
				 filename );

				break;

			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_INVALID_RESOURCE,
				 "%s: no such file: %ls.",
				 function,
				 filename );

				break;

			default:
				if( libsmdev_error_string_copy_from_error_number(
				     error_string,
				     LIBSMDEV_ERROR_STRING_DEFAULT_SIZE,
				     error_code,
				     error ) != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open file: %ls with error: %" PRIs_LIBSMDEV_SYSTEM "",
					 function,
					 filename,
					 error_string );
				}
				else
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open file: %ls.",
					 function,
					 filename );
				}
				break;
		}
		return( -1 );
	}
	/* Use the GetFileType function to rule out certain file types
	 * like pipes, sockets, etc.
	 */
	file_type = GetFileType(
	             file_handle );

	if( file_type != FILE_TYPE_DISK )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file type.",
		 function );

		CloseHandle(
		 file_handle );

		return( -1 );
	}
	if( ( filename[ 0 ] == '\\' )
	 && ( filename[ 1 ] == '\\' )
	 && ( filename[ 2 ] == '.' )
	 && ( filename[ 3 ] == '\\' ) )
	{
		result = 1;
	}
	if( CloseHandle(
	     file_handle ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file: %ls.",
		 function,
		 filename );

		return( -1 );
	}
#else
	/* Convert the filename to a narrow string
	 * if the platform has no wide character open function
	 */
	filename_length = wide_string_length(
	                   filename );

	if( libsmdev_system_narrow_string_codepage == 0 )
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
		          libsmdev_system_narrow_string_codepage,
		          &narrow_filename_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          libsmdev_system_narrow_string_codepage,
		          &narrow_filename_size,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow character filename size.",
		 function );

		return( -1 );
	}
	narrow_filename = (char *) memory_allocate(
				    sizeof( char ) * narrow_filename_size );

	if( narrow_filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create narrow character filename.",
		 function );

		return( -1 );
	}
	if( libsmdev_system_narrow_string_codepage == 0 )
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
		          libsmdev_system_narrow_string_codepage,
		          (libuna_utf32_character_t *) filename,
		          filename_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) narrow_filename,
		          narrow_filename_size,
		          libsmdev_system_narrow_string_codepage,
		          (libuna_utf16_character_t *) filename,
		          filename_length + 1,
		          error );
#else
#error Unsupported size of wchar_t
#endif /* SIZEOF_WCHAR_T */
	}
	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow character filename.",
		 function );

		memory_free(
		 narrow_filename );

		return( -1 );
	}
#if defined( HAVE_GLIB_H )
	file_descriptor = g_open(
	                   narrow_filename,
	                   file_io_flags,
	                   0644 );
#else
	file_descriptor = open(
	                   narrow_filename,
	                   O_RDONLY,
	                   0644 );

#endif /* HAVE_GLIB_H */

	memory_free(
	 narrow_filename );

	if( file_descriptor == -1 )
	{
		switch( errno )
		{
			case EACCES:
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_ACCESS_DENIED,
				 "%s: access denied to file: %s.",
				 function,
				 filename );

				break;

			case ENOENT:
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_INVALID_RESOURCE,
				 "%s: no such file: %s.",
				 function,
				 filename );

				break;

			default:
				if( libsmdev_error_string_copy_from_error_number(
				     error_string,
				     LIBSMDEV_ERROR_STRING_DEFAULT_SIZE,
				     errno,
				     error ) != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open file: %s with error: %" PRIs_LIBSMDEV_SYSTEM "",
					 function,
					 filename,
					 error_string );
				}
				else
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to open file: %s.",
					 function,
					 filename );
				}
				break;
		}
		return( -1 );
	}
	if( fstat(
	     file_descriptor,
	     &file_stat ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_GENERIC,
		 "%s: unable to determine file status information.",
		 function );

		close(
		 file_descriptor );

		return( -1 );
	}
	if( S_ISBLK( file_stat.st_mode )
	 || S_ISCHR( file_stat.st_mode ) )
	{
		result = 1;
	}
	if( close(
	     file_descriptor ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file: %s.",
		 function,
		 filename );

		return( -1 );
	}

#endif
	return( result );
}

#endif

