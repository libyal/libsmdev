/*
 * Library handle type test program
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

#include <common.h>
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smdev_test_functions.h"
#include "smdev_test_getopt.h"
#include "smdev_test_libcerror.h"
#include "smdev_test_libsmdev.h"
#include "smdev_test_macros.h"
#include "smdev_test_memory.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make smdev_test_handle generate verbose output
#define SMDEV_TEST_HANDLE_VERBOSE
 */

/* Creates and opens a source handle
 * Returns 1 if successful or -1 on error
 */
int smdev_test_handle_open_source(
     libsmdev_handle_t **handle,
     const system_character_t *source,
     libcerror_error_t **error )
{
	static char *function = "smdev_test_handle_open_source";
	int result            = 0;

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
	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( libsmdev_handle_initialize(
	     handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmdev_handle_open_wide(
	          *handle,
	          source,
	          LIBSMDEV_OPEN_READ,
	          error );
#else
	result = libsmdev_handle_open(
	          *handle,
	          source,
	          LIBSMDEV_OPEN_READ,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *handle != NULL )
	{
		libsmdev_handle_free(
		 handle,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source handle
 * Returns 1 if successful or -1 on error
 */
int smdev_test_handle_close_source(
     libsmdev_handle_t **handle,
     libcerror_error_t **error )
{
	static char *function = "smdev_test_handle_close_source";
	int result            = 0;

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
	if( libsmdev_handle_free(
	     handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libsmdev_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libsmdev_handle_t *handle       = NULL;
	int result                      = 0;

#if defined( HAVE_SMDEV_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libsmdev_handle_initialize(
	          &handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmdev_handle_free(
	          &handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmdev_handle_initialize(
	          NULL,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	handle = (libsmdev_handle_t *) 0x12345678UL;

	result = libsmdev_handle_initialize(
	          &handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	handle = NULL;

#if defined( HAVE_SMDEV_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libsmdev_handle_initialize with malloc failing
		 */
		smdev_test_malloc_attempts_before_fail = test_number;

		result = libsmdev_handle_initialize(
		          &handle,
		          &error );

		if( smdev_test_malloc_attempts_before_fail != -1 )
		{
			smdev_test_malloc_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libsmdev_handle_free(
				 &handle,
				 NULL );
			}
		}
		else
		{
			SMDEV_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			SMDEV_TEST_ASSERT_IS_NULL(
			 "handle",
			 handle );

			SMDEV_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libsmdev_handle_initialize with memset failing
		 */
		smdev_test_memset_attempts_before_fail = test_number;

		result = libsmdev_handle_initialize(
		          &handle,
		          &error );

		if( smdev_test_memset_attempts_before_fail != -1 )
		{
			smdev_test_memset_attempts_before_fail = -1;

			if( handle != NULL )
			{
				libsmdev_handle_free(
				 &handle,
				 NULL );
			}
		}
		else
		{
			SMDEV_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			SMDEV_TEST_ASSERT_IS_NULL(
			 "handle",
			 handle );

			SMDEV_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_SMDEV_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmdev_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmdev_handle_free function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libsmdev_handle_free(
	          NULL,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmdev_handle_open function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error  = NULL;
	libsmdev_handle_t *handle = NULL;
	int result                = 0;

	/* Initialize test
	 */
	result = smdev_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmdev_handle_initialize(
	          &handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libsmdev_handle_open(
	          handle,
	          narrow_source,
	          LIBSMDEV_OPEN_READ,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmdev_handle_open(
	          handle,
	          narrow_source,
	          LIBSMDEV_OPEN_READ,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libsmdev_handle_free(
	          &handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmdev_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libsmdev_handle_open_wide function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error  = NULL;
	libsmdev_handle_t *handle = NULL;
	int result                = 0;

	/* Initialize test
	 */
	result = smdev_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmdev_handle_initialize(
	          &handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libsmdev_handle_open_wide(
	          handle,
	          wide_source,
	          LIBSMDEV_OPEN_READ,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmdev_handle_open_wide(
	          handle,
	          wide_source,
	          LIBSMDEV_OPEN_READ,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libsmdev_handle_free(
	          &handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmdev_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libsmdev_handle_close function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libsmdev_handle_close(
	          NULL,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmdev_handle_open and libsmdev_handle_close functions
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error  = NULL;
	libsmdev_handle_t *handle = NULL;
	int result                = 0;

	/* Initialize test
	 */
	result = libsmdev_handle_initialize(
	          &handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "handle",
	 handle );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmdev_handle_open_wide(
	          handle,
	          source,
	          LIBSMDEV_OPEN_READ,
	          &error );
#else
	result = libsmdev_handle_open(
	          handle,
	          source,
	          LIBSMDEV_OPEN_READ,
	          &error );
#endif

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmdev_handle_close(
	          handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libsmdev_handle_open_wide(
	          handle,
	          source,
	          LIBSMDEV_OPEN_READ,
	          &error );
#else
	result = libsmdev_handle_open(
	          handle,
	          source,
	          LIBSMDEV_OPEN_READ,
	          &error );
#endif

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmdev_handle_close(
	          handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libsmdev_handle_free(
	          &handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "handle",
	 handle );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		libsmdev_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmdev_handle_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_signal_abort(
     libsmdev_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmdev_handle_signal_abort(
	          handle,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmdev_handle_signal_abort(
	          NULL,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmdev_handle_get_offset function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_get_offset(
     libsmdev_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int offset_is_set        = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmdev_handle_get_offset(
	          handle,
	          &offset,
	          &error );

	SMDEV_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset_is_set = result;

	/* Test error cases
	 */
	result = libsmdev_handle_get_offset(
	          NULL,
	          &offset,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( offset_is_set != 0 )
	{
		result = libsmdev_handle_get_offset(
		          handle,
		          NULL,
		          &error );

		SMDEV_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		SMDEV_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmdev_handle_get_filename_size function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_get_filename_size(
     libsmdev_handle_t *handle )
{
	libcerror_error_t *error = NULL;
	size_t filename_size     = 0;
	int filename_size_is_set = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmdev_handle_get_filename_size(
	          handle,
	          &filename_size,
	          &error );

	SMDEV_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	filename_size_is_set = result;

	/* Test error cases
	 */
	result = libsmdev_handle_get_filename_size(
	          NULL,
	          &filename_size,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( filename_size_is_set != 0 )
	{
		result = libsmdev_handle_get_filename_size(
		          handle,
		          NULL,
		          &error );

		SMDEV_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		SMDEV_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libsmdev_handle_get_filename_size_wide function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_handle_get_filename_size_wide(
     libsmdev_handle_t *handle )
{
	libcerror_error_t *error      = NULL;
	size_t filename_size_wide     = 0;
	int filename_size_wide_is_set = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libsmdev_handle_get_filename_size_wide(
	          handle,
	          &filename_size_wide,
	          &error );

	SMDEV_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	filename_size_wide_is_set = result;

	/* Test error cases
	 */
	result = libsmdev_handle_get_filename_size_wide(
	          NULL,
	          &filename_size_wide,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	if( filename_size_wide_is_set != 0 )
	{
		result = libsmdev_handle_get_filename_size_wide(
		          handle,
		          NULL,
		          &error );

		SMDEV_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		SMDEV_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libcerror_error_t *error   = NULL;
	libsmdev_handle_t *handle  = NULL;
	system_character_t *source = NULL;
	system_integer_t option    = 0;
	int result                 = 0;

	while( ( option = smdev_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( SMDEV_TEST_HANDLE_VERBOSE )
	libsmdev_notify_set_verbose(
	 1 );
	libsmdev_notify_set_stream(
	 stderr,
	 NULL );
#endif

	SMDEV_TEST_RUN(
	 "libsmdev_handle_initialize",
	 smdev_test_handle_initialize );

	SMDEV_TEST_RUN(
	 "libsmdev_handle_free",
	 smdev_test_handle_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		SMDEV_TEST_RUN_WITH_ARGS(
		 "libsmdev_handle_open",
		 smdev_test_handle_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		SMDEV_TEST_RUN_WITH_ARGS(
		 "libsmdev_handle_open_wide",
		 smdev_test_handle_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBSMDEV_HAVE_BFIO )

		/* TODO add test for libsmdev_handle_open_file_io_handle */

#endif /* defined( LIBSMDEV_HAVE_BFIO ) */

		SMDEV_TEST_RUN(
		 "libsmdev_handle_close",
		 smdev_test_handle_close );

		SMDEV_TEST_RUN_WITH_ARGS(
		 "libsmdev_handle_open_close",
		 smdev_test_handle_open_close,
		 source );

		/* Initialize test
		 */
		result = smdev_test_handle_open_source(
		          &handle,
		          source,
		          &error );

		SMDEV_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        SMDEV_TEST_ASSERT_IS_NOT_NULL(
	         "handle",
	         handle );

	        SMDEV_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		SMDEV_TEST_RUN_WITH_ARGS(
		 "libsmdev_handle_signal_abort",
		 smdev_test_handle_signal_abort,
		 handle );

		/* TODO: add tests for libsmdev_handle_read_buffer */

		/* TODO: add tests for libsmdev_handle_read_buffer_at_offset */

		/* TODO: add tests for libsmdev_handle_write_buffer */

		/* TODO: add tests for libsmdev_handle_write_buffer_at_offset */

		/* TODO: add tests for libsmdev_handle_seek_offset */

		SMDEV_TEST_RUN_WITH_ARGS(
		 "libsmdev_handle_get_offset",
		 smdev_test_handle_get_offset,
		 handle );

		SMDEV_TEST_RUN_WITH_ARGS(
		 "libsmdev_handle_get_filename_size",
		 smdev_test_handle_get_filename_size,
		 handle );

		/* TODO: add tests for libsmdev_handle_get_filename */

#if defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT )

		/* TODO: add tests for libsmdev_handle_set_filename */

#endif /* defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT ) */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		SMDEV_TEST_RUN_WITH_ARGS(
		 "libsmdev_handle_get_filename_size_wide",
		 smdev_test_handle_get_filename_size_wide,
		 handle );

		/* TODO: add tests for libsmdev_handle_get_filename_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT )

		/* TODO: add tests for libsmdev_handle_set_filename_wide */

		/* TODO: add tests for libsmdev_handle_append_session */

		/* TODO: add tests for libsmdev_handle_append_lead_out */

		/* TODO: add tests for libsmdev_handle_append_track */

#endif /* defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT ) */

		/* Clean up
		 */
		result = smdev_test_handle_close_source(
		          &handle,
		          &error );

		SMDEV_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		SMDEV_TEST_ASSERT_IS_NULL(
	         "handle",
	         handle );

	        SMDEV_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( handle != NULL )
	{
		smdev_test_handle_close_source(
		 &handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

