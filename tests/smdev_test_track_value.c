/*
 * Library track_value type test program
 *
 * Copyright (C) 2010-2022, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smdev_test_libcerror.h"
#include "smdev_test_libsmdev.h"
#include "smdev_test_macros.h"
#include "smdev_test_memory.h"
#include "smdev_test_unused.h"

#include "../libsmdev/libsmdev_track_value.h"

#if defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT )

/* Tests the libsmdev_track_value_initialize function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_track_value_initialize(
     void )
{
	libcerror_error_t *error            = NULL;
	libsmdev_track_value_t *track_value = NULL;
	int result                          = 0;

#if defined( HAVE_SMDEV_TEST_MEMORY )
	int number_of_malloc_fail_tests     = 1;
	int number_of_memset_fail_tests     = 1;
	int test_number                     = 0;
#endif

	/* Test regular cases
	 */
	result = libsmdev_track_value_initialize(
	          &track_value,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "track_value",
	 track_value );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmdev_track_value_free(
	          &track_value,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "track_value",
	 track_value );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmdev_track_value_initialize(
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

	track_value = (libsmdev_track_value_t *) 0x12345678UL;

	result = libsmdev_track_value_initialize(
	          &track_value,
	          &error );

	track_value = NULL;

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_SMDEV_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libsmdev_track_value_initialize with malloc failing
		 */
		smdev_test_malloc_attempts_before_fail = test_number;

		result = libsmdev_track_value_initialize(
		          &track_value,
		          &error );

		if( smdev_test_malloc_attempts_before_fail != -1 )
		{
			smdev_test_malloc_attempts_before_fail = -1;

			if( track_value != NULL )
			{
				libsmdev_track_value_free(
				 &track_value,
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
			 "track_value",
			 track_value );

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
		/* Test libsmdev_track_value_initialize with memset failing
		 */
		smdev_test_memset_attempts_before_fail = test_number;

		result = libsmdev_track_value_initialize(
		          &track_value,
		          &error );

		if( smdev_test_memset_attempts_before_fail != -1 )
		{
			smdev_test_memset_attempts_before_fail = -1;

			if( track_value != NULL )
			{
				libsmdev_track_value_free(
				 &track_value,
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
			 "track_value",
			 track_value );

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
	if( track_value != NULL )
	{
		libsmdev_track_value_free(
		 &track_value,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmdev_track_value_free function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_track_value_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libsmdev_track_value_free(
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

/* Tests the libsmdev_track_value_get function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_track_value_get(
     void )
{
	libcerror_error_t *error            = NULL;
	libsmdev_track_value_t *track_value = NULL;
	uint64_t number_of_sectors          = 0;
	uint64_t start_sector               = 0;
	uint8_t type                        = 0;
	int result                          = 0;

	/* Initialize test
	 */
	result = libsmdev_track_value_initialize(
	          &track_value,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "track_value",
	 track_value );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmdev_track_value_get(
	          track_value,
	          &start_sector,
	          &number_of_sectors,
	          &type,
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
	result = libsmdev_track_value_get(
	          NULL,
	          &start_sector,
	          &number_of_sectors,
	          &type,
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

	result = libsmdev_track_value_get(
	          track_value,
	          NULL,
	          &number_of_sectors,
	          &type,
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

	result = libsmdev_track_value_get(
	          track_value,
	          &start_sector,
	          NULL,
	          &type,
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

	result = libsmdev_track_value_get(
	          track_value,
	          &start_sector,
	          &number_of_sectors,
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

	/* Clean up
	 */
	result = libsmdev_track_value_free(
	          &track_value,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "track_value",
	 track_value );

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
	if( track_value != NULL )
	{
		libsmdev_track_value_free(
		 &track_value,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmdev_track_value_set function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_track_value_set(
     void )
{
	libcerror_error_t *error            = NULL;
	libsmdev_track_value_t *track_value = NULL;
	int result                          = 0;

	/* Initialize test
	 */
	result = libsmdev_track_value_initialize(
	          &track_value,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "track_value",
	 track_value );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmdev_track_value_set(
	          track_value,
	          0,
	          64,
	          LIBSMDEV_TRACK_TYPE_MODE1_2048,
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
	result = libsmdev_track_value_set(
	          NULL,
	          0,
	          64,
	          LIBSMDEV_TRACK_TYPE_MODE1_2048,
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

	result = libsmdev_track_value_set(
	          track_value,
	          (uint64_t) INT64_MAX + 1,
	          64,
	          LIBSMDEV_TRACK_TYPE_MODE1_2048,
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

	result = libsmdev_track_value_set(
	          track_value,
	          0,
	          (uint64_t) INT64_MAX + 1,
	          LIBSMDEV_TRACK_TYPE_MODE1_2048,
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

	result = libsmdev_track_value_set(
	          track_value,
	          0,
	          64,
	          0xff,
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
	result = libsmdev_track_value_free(
	          &track_value,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "track_value",
	 track_value );

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
	if( track_value != NULL )
	{
		libsmdev_track_value_free(
		 &track_value,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmdev_track_value_get_bytes_per_sector function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_track_value_get_bytes_per_sector(
     void )
{
	libcerror_error_t *error            = NULL;
	libsmdev_track_value_t *track_value = NULL;
	uint32_t bytes_per_sector           = 0;
	int result                          = 0;

	/* Initialize test
	 */
	result = libsmdev_track_value_initialize(
	          &track_value,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "track_value",
	 track_value );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmdev_track_value_get_bytes_per_sector(
	          track_value,
	          &bytes_per_sector,
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
	result = libsmdev_track_value_get_bytes_per_sector(
	          NULL,
	          &bytes_per_sector,
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

	result = libsmdev_track_value_get_bytes_per_sector(
	          track_value,
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

	/* Clean up
	 */
	result = libsmdev_track_value_free(
	          &track_value,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "track_value",
	 track_value );

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
	if( track_value != NULL )
	{
		libsmdev_track_value_free(
		 &track_value,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc SMDEV_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] SMDEV_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc SMDEV_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] SMDEV_TEST_ATTRIBUTE_UNUSED )
#endif
{
	SMDEV_TEST_UNREFERENCED_PARAMETER( argc )
	SMDEV_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT )

	SMDEV_TEST_RUN(
	 "libsmdev_track_value_initialize",
	 smdev_test_track_value_initialize );

	SMDEV_TEST_RUN(
	 "libsmdev_track_value_free",
	 smdev_test_track_value_free );

	SMDEV_TEST_RUN(
	 "libsmdev_track_value_get",
	 smdev_test_track_value_get );

	SMDEV_TEST_RUN(
	 "libsmdev_track_value_set",
	 smdev_test_track_value_set );

	SMDEV_TEST_RUN(
	 "libsmdev_track_value_get_bytes_per_sector",
	 smdev_test_track_value_get_bytes_per_sector );

#endif /* defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

