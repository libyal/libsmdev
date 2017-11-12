/*
 * Library sector_range type test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smdev_test_libcerror.h"
#include "smdev_test_libsmdev.h"
#include "smdev_test_macros.h"
#include "smdev_test_memory.h"
#include "smdev_test_unused.h"

#include "../libsmdev/libsmdev_sector_range.h"

#if defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT )

/* Tests the libsmdev_sector_range_initialize function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_sector_range_initialize(
     void )
{
	libcerror_error_t *error              = NULL;
	libsmdev_sector_range_t *sector_range = NULL;
	int result                            = 0;

#if defined( HAVE_SMDEV_TEST_MEMORY )
	int number_of_malloc_fail_tests       = 1;
	int number_of_memset_fail_tests       = 1;
	int test_number                       = 0;
#endif

	/* Test regular cases
	 */
	result = libsmdev_sector_range_initialize(
	          &sector_range,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "sector_range",
	 sector_range );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmdev_sector_range_free(
	          &sector_range,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "sector_range",
	 sector_range );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmdev_sector_range_initialize(
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

	sector_range = (libsmdev_sector_range_t *) 0x12345678UL;

	result = libsmdev_sector_range_initialize(
	          &sector_range,
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

	sector_range = NULL;

#if defined( HAVE_SMDEV_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libsmdev_sector_range_initialize with malloc failing
		 */
		smdev_test_malloc_attempts_before_fail = test_number;

		result = libsmdev_sector_range_initialize(
		          &sector_range,
		          &error );

		if( smdev_test_malloc_attempts_before_fail != -1 )
		{
			smdev_test_malloc_attempts_before_fail = -1;

			if( sector_range != NULL )
			{
				libsmdev_sector_range_free(
				 &sector_range,
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
			 "sector_range",
			 sector_range );

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
		/* Test libsmdev_sector_range_initialize with memset failing
		 */
		smdev_test_memset_attempts_before_fail = test_number;

		result = libsmdev_sector_range_initialize(
		          &sector_range,
		          &error );

		if( smdev_test_memset_attempts_before_fail != -1 )
		{
			smdev_test_memset_attempts_before_fail = -1;

			if( sector_range != NULL )
			{
				libsmdev_sector_range_free(
				 &sector_range,
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
			 "sector_range",
			 sector_range );

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
	if( sector_range != NULL )
	{
		libsmdev_sector_range_free(
		 &sector_range,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmdev_sector_range_free function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_sector_range_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libsmdev_sector_range_free(
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

/* Tests the libsmdev_sector_range_clone function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_sector_range_clone(
     void )
{
	libcerror_error_t *error                          = NULL;
	libsmdev_sector_range_t *destination_sector_range = NULL;
	libsmdev_sector_range_t *source_sector_range      = NULL;
	int result                                        = 0;

	/* Initialize test
	 */
	result = libsmdev_sector_range_initialize(
	          &source_sector_range,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "source_sector_range",
	 source_sector_range );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmdev_sector_range_clone(
	          &destination_sector_range,
	          source_sector_range,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "destination_sector_range",
	 destination_sector_range );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmdev_sector_range_free(
	          &destination_sector_range,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "destination_sector_range",
	 destination_sector_range );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libsmdev_sector_range_clone(
	          &destination_sector_range,
	          NULL,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "destination_sector_range",
	 destination_sector_range );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libsmdev_sector_range_clone(
	          NULL,
	          source_sector_range,
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

	destination_sector_range = (libsmdev_sector_range_t *) 0x12345678UL;

	result = libsmdev_sector_range_clone(
	          &destination_sector_range,
	          source_sector_range,
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

	destination_sector_range = NULL;

#if defined( HAVE_SMDEV_TEST_MEMORY )

	/* Test libsmdev_sector_range_clone with malloc failing
	 */
	smdev_test_malloc_attempts_before_fail = 0;

	result = libsmdev_sector_range_clone(
	          &destination_sector_range,
	          source_sector_range,
	          &error );

	if( smdev_test_malloc_attempts_before_fail != -1 )
	{
		smdev_test_malloc_attempts_before_fail = -1;

		if( destination_sector_range != NULL )
		{
			libsmdev_sector_range_free(
			 &destination_sector_range,
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
		 "destination_sector_range",
		 destination_sector_range );

		SMDEV_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}

	/* Test libsmdev_sector_range_clone with memcpy failing
	 */
	smdev_test_memcpy_attempts_before_fail = 0;

	result = libsmdev_sector_range_clone(
	          &destination_sector_range,
	          source_sector_range,
	          &error );

	if( smdev_test_memcpy_attempts_before_fail != -1 )
	{
		smdev_test_memcpy_attempts_before_fail = -1;

		if( destination_sector_range != NULL )
		{
			libsmdev_sector_range_free(
			 &destination_sector_range,
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
		 "destination_sector_range",
		 destination_sector_range );

		SMDEV_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_SMDEV_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libsmdev_sector_range_free(
	          &source_sector_range,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "source_sector_range",
	 source_sector_range );

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
	if( destination_sector_range != NULL )
	{
		libsmdev_sector_range_free(
		 &destination_sector_range,
		 NULL );
	}
	if( source_sector_range != NULL )
	{
		libsmdev_sector_range_free(
		 &source_sector_range,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmdev_sector_range_get function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_sector_range_get(
     void )
{
	libcerror_error_t *error              = NULL;
	libsmdev_sector_range_t *sector_range = NULL;
	uint64_t number_of_sectors            = 0;
	uint64_t start_sector                 = 0;
	int result                            = 0;

	/* Initialize test
	 */
	result = libsmdev_sector_range_initialize(
	          &sector_range,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "sector_range",
	 sector_range );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libsmdev_sector_range_get(
	          sector_range,
	          &start_sector,
	          &number_of_sectors,
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
	result = libsmdev_sector_range_get(
	          NULL,
	          &start_sector,
	          &number_of_sectors,
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

	result = libsmdev_sector_range_get(
	          sector_range,
	          NULL,
	          &number_of_sectors,
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

	result = libsmdev_sector_range_get(
	          sector_range,
	          &start_sector,
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
	result = libsmdev_sector_range_free(
	          &sector_range,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "sector_range",
	 sector_range );

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
	if( sector_range != NULL )
	{
		libsmdev_sector_range_free(
		 &sector_range,
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
	 "libsmdev_sector_range_initialize",
	 smdev_test_sector_range_initialize );

	SMDEV_TEST_RUN(
	 "libsmdev_sector_range_free",
	 smdev_test_sector_range_free );

	SMDEV_TEST_RUN(
	 "libsmdev_sector_range_clone",
	 smdev_test_sector_range_clone );

	SMDEV_TEST_RUN(
	 "libsmdev_sector_range_get",
	 smdev_test_sector_range_get );

	/* TODO: add tests for libsmdev_sector_range_set */

#endif /* defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

