/*
 * Library ata functions test program
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

#if defined( HAVE_CYGWIN_HDREG_H )
#include <cygwin/hdreg.h>
#endif

#if defined( HAVE_LINUX_HDREG_H )
#include <linux/hdreg.h>
#endif

#include "smdev_test_libcerror.h"
#include "smdev_test_libcfile.h"
#include "smdev_test_libsmdev.h"
#include "smdev_test_macros.h"
#include "smdev_test_unused.h"

#include "../libsmdev/libsmdev_ata.h"

#if defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT )

#if defined( HDIO_GET_IDENTITY )

/* Tests the libsmdev_ata_get_device_configuration function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_ata_get_device_configuration(
     void )
{
	struct hd_driveid device_configuration;

	libcerror_error_t *error     = NULL;
	libcfile_file_t *device_file = NULL;
	int result                   = 0;

	/* Initialize test
	 */
	result = libcfile_file_initialize(
	          &device_file,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NOT_NULL(
	 "device_file",
	 device_file );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
#if defined( WINAPI )
	result = libcfile_file_open_wide(
	          device_file,
	          L"\\\\.\\PhysicalDrive0",
	          LIBCFILE_OPEN_READ,
	          NULL );
#else
	result = libcfile_file_open(
	          device_file,
	          "/dev/sda",
	          LIBCFILE_OPEN_READ,
	          NULL );
#endif
	if( result == 1 )
	{
		result = libsmdev_ata_get_device_configuration(
		          device_file,
		          &device_configuration,
		          &error );

		SMDEV_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		SMDEV_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libcfile_file_close(
		          device_file,
		          &error );

		SMDEV_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		SMDEV_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test on a closed file
	 */
	result = libsmdev_ata_get_device_configuration(
	          device_file,
	          &device_configuration,
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
	result = libsmdev_ata_get_device_configuration(
	          NULL,
	          &device_configuration,
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

	result = libsmdev_ata_get_device_configuration(
	          device_file,
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
	result = libcfile_file_free(
	          &device_file,
	          &error );

	SMDEV_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	SMDEV_TEST_ASSERT_IS_NULL(
	 "device_file",
	 device_file );

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
	if( device_file != NULL )
	{
		libcfile_file_free(
		 &device_file,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HDIO_GET_IDENTITY ) */

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

#if defined( HDIO_GET_IDENTITY )

	SMDEV_TEST_RUN(
	 "libsmdev_ata_get_device_configuration",
	 smdev_test_ata_get_device_configuration );

#endif /* defined( HDIO_GET_IDENTITY ) */

#endif /* defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

