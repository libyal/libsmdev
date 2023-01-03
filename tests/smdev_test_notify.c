/*
 * Library notification functions test program
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include "smdev_test_unused.h"

/* Tests the libsmdev_notify_set_verbose function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_notify_set_verbose(
     void )
{
	/* Test invocation of function only
	 */
	libsmdev_notify_set_verbose(
	 0 );

	return( 1 );
}

/* Tests the libsmdev_notify_set_stream function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_notify_set_stream(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmdev_notify_set_stream(
	          NULL,
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
/* TODO test libcnotify_stream_set failure */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libsmdev_notify_stream_open function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_notify_stream_open(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmdev_notify_stream_open(
	          "notify_stream.log",
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
	result = libsmdev_notify_stream_open(
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
	result = libsmdev_notify_stream_close(
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

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
	return( 0 );
}

/* Tests the libsmdev_notify_stream_close function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_notify_stream_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libsmdev_notify_stream_close(
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
/* TODO test libcnotify_stream_close failure */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

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

	SMDEV_TEST_RUN(
	 "libsmdev_notify_set_verbose",
	 smdev_test_notify_set_verbose )

	SMDEV_TEST_RUN(
	 "libsmdev_notify_set_stream",
	 smdev_test_notify_set_stream )

	SMDEV_TEST_RUN(
	 "libsmdev_notify_stream_open",
	 smdev_test_notify_stream_open )

	SMDEV_TEST_RUN(
	 "libsmdev_notify_stream_close",
	 smdev_test_notify_stream_close )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

