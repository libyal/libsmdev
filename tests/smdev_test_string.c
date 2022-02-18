/*
 * Library string functions test program
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_CYGWIN_HDREG_H )
#include <cygwin/hdreg.h>
#endif

#if defined( HAVE_LINUX_HDREG_H )
#include <linux/hdreg.h>
#endif

#include "smdev_test_libcerror.h"
#include "smdev_test_libsmdev.h"
#include "smdev_test_macros.h"
#include "smdev_test_unused.h"

#include "../libsmdev/libsmdev_string.h"

#if defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT )

/* Tests the libsmdev_string_trim_copy_from_byte_stream function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_string_trim_copy_from_byte_stream(
     void )
{
	uint8_t string[ 32 ];

	libcerror_error_t *error = NULL;
	uint8_t *byte_stream     = (uint8_t *) " TEST ";
	ssize_t result           = 0;
	int compare_result       = 0;

	/* Test regular cases
	 */
	result = libsmdev_string_trim_copy_from_byte_stream(
	          string,
	          32,
	          byte_stream,
	          7,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_SSIZE(
	 "result",
	 result,
	 (ssize_t) 5 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	compare_result = memory_compare(
	                  string,
	                  "TEST",
	                  5 );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "compare_result",
	 compare_result,
	 0 );

	/* Test error cases
	 */
	result = libsmdev_string_trim_copy_from_byte_stream(
	          NULL,
	          32,
	          byte_stream,
	          7,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_SSIZE(
	 "result",
	 result,
	 (ssize_t) -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmdev_string_trim_copy_from_byte_stream(
	          string,
	          (size_t) SSIZE_MAX + 1,
	          byte_stream,
	          7,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_SSIZE(
	 "result",
	 result,
	 (ssize_t) -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmdev_string_trim_copy_from_byte_stream(
	          string,
	          32,
	          NULL,
	          7,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_SSIZE(
	 "result",
	 result,
	 (ssize_t) -1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libsmdev_string_trim_copy_from_byte_stream(
	          string,
	          32,
	          byte_stream,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_SSIZE(
	 "result",
	 result,
	 (ssize_t) -1 );

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
	 "libsmdev_string_trim_copy_from_byte_stream",
	 smdev_test_string_trim_copy_from_byte_stream );

#endif /* defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

