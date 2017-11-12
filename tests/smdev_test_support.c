/*
 * Library support functions test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "smdev_test_libcerror.h"
#include "smdev_test_libsmdev.h"
#include "smdev_test_macros.h"
#include "smdev_test_unused.h"

/* Tests the libsmdev_get_version function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_get_version(
     void )
{
	const char *version_string = NULL;
	int result                 = 0;

	version_string = libsmdev_get_version();

	result = narrow_string_compare(
	          version_string,
	          LIBSMDEV_VERSION_STRING,
	          9 );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libsmdev_get_access_flags_read function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_get_access_flags_read(
     void )
{
	int access_flags = 0;

	access_flags = libsmdev_get_access_flags_read();

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 LIBSMDEV_ACCESS_FLAG_READ );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libsmdev_get_codepage function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_get_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int result               = 0;

	result = libsmdev_get_codepage(
	          &codepage,
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
	result = libsmdev_get_codepage(
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

/* Tests the libsmdev_set_codepage function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_set_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = libsmdev_set_codepage(
	          0,
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
	result = libsmdev_set_codepage(
	          -1,
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
	 "libsmdev_get_version",
	 smdev_test_get_version );

	SMDEV_TEST_RUN(
	 "libsmdev_get_access_flags_read",
	 smdev_test_get_access_flags_read );

	SMDEV_TEST_RUN(
	 "libsmdev_get_codepage",
	 smdev_test_get_codepage );

	SMDEV_TEST_RUN(
	 "libsmdev_set_codepage",
	 smdev_test_set_codepage );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

