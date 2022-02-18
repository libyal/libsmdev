/*
 * Library optical_disc functions test program
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

#include "../libsmdev/libsmdev_handle.h"
#include "../libsmdev/libsmdev_optical_disc.h"

#if defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT )

#if defined( HAVE_LINUX_CDROM_H )

/* Tests the libsmdev_optical_disc_get_table_of_contents function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_optical_disc_get_table_of_contents(
     void )
{
	libcerror_error_t *error     = NULL;
	libcfile_file_t *device_file = NULL;
	libsmdev_handle_t *handle    = NULL;
	int result                   = 0;

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
	          L"\\\\.\\CDROM0",
	          LIBCFILE_OPEN_READ,
	          NULL );
#else
	result = libcfile_file_open(
	          device_file,
	          "/dev/cdrom",
	          LIBCFILE_OPEN_READ,
	          NULL );
#endif

	if( result == 1 )
	{
		result = libsmdev_optical_disc_get_table_of_contents(
		          device_file,
		          (libsmdev_internal_handle_t *) handle,
		          &error );

		SMDEV_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

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
	result = libsmdev_optical_disc_get_table_of_contents(
	          device_file,
	          (libsmdev_internal_handle_t *) handle,
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

	/* Test error cases
	 */
	result = libsmdev_optical_disc_get_table_of_contents(
	          NULL,
	          (libsmdev_internal_handle_t *) handle,
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

	result = libsmdev_optical_disc_get_table_of_contents(
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
	if( device_file != NULL )
	{
		libcfile_file_free(
		 &device_file,
		 NULL );
	}
	if( handle != NULL )
	{
		libsmdev_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmdev_optical_disc_get_table_of_contents_ioctl function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_optical_disc_get_table_of_contents_ioctl(
     void )
{
	libcerror_error_t *error     = NULL;
	libcfile_file_t *device_file = NULL;
	libsmdev_handle_t *handle    = NULL;
	int result                   = 0;

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
	          L"\\\\.\\CDROM0",
	          LIBCFILE_OPEN_READ,
	          NULL );
#else
	result = libcfile_file_open(
	          device_file,
	          "/dev/cdrom",
	          LIBCFILE_OPEN_READ,
	          NULL );
#endif

	if( result == 1 )
	{
		result = libsmdev_optical_disc_get_table_of_contents_ioctl(
		          device_file,
		          (libsmdev_internal_handle_t *) handle,
		          &error );

		SMDEV_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

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
	result = libsmdev_optical_disc_get_table_of_contents_ioctl(
	          device_file,
	          (libsmdev_internal_handle_t *) handle,
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
	result = libsmdev_optical_disc_get_table_of_contents_ioctl(
	          NULL,
	          (libsmdev_internal_handle_t *) handle,
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

	result = libsmdev_optical_disc_get_table_of_contents_ioctl(
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
	if( device_file != NULL )
	{
		libcfile_file_free(
		 &device_file,
		 NULL );
	}
	if( handle != NULL )
	{
		libsmdev_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libsmdev_optical_disc_get_table_of_contents_scsi function
 * Returns 1 if successful or 0 if not
 */
int smdev_test_optical_disc_get_table_of_contents_scsi(
     void )
{
	libcerror_error_t *error     = NULL;
	libcfile_file_t *device_file = NULL;
	libsmdev_handle_t *handle    = NULL;
	int result                   = 0;

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
	          L"\\\\.\\CDROM0",
	          LIBCFILE_OPEN_READ,
	          NULL );
#else
	result = libcfile_file_open(
	          device_file,
	          "/dev/cdrom",
	          LIBCFILE_OPEN_READ,
	          NULL );
#endif

	if( result == 1 )
	{
		result = libsmdev_optical_disc_get_table_of_contents_scsi(
		          device_file,
		          (libsmdev_internal_handle_t *) handle,
		          &error );

		SMDEV_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

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
	result = libsmdev_optical_disc_get_table_of_contents_scsi(
	          device_file,
	          (libsmdev_internal_handle_t *) handle,
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

	/* Test error cases
	 */
	result = libsmdev_optical_disc_get_table_of_contents_scsi(
	          NULL,
	          (libsmdev_internal_handle_t *) handle,
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

	result = libsmdev_optical_disc_get_table_of_contents_scsi(
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
	if( device_file != NULL )
	{
		libcfile_file_free(
		 &device_file,
		 NULL );
	}
	if( handle != NULL )
	{
		libsmdev_handle_free(
		 &handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_LINUX_CDROM_H ) */

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

#if defined( HAVE_LINUX_CDROM_H )

	SMDEV_TEST_RUN(
	 "libsmdev_optical_disc_get_table_of_contents",
	 smdev_test_optical_disc_get_table_of_contents );

	SMDEV_TEST_RUN(
	 "libsmdev_optical_disc_get_table_of_contents_ioctl",
	 smdev_test_optical_disc_get_table_of_contents_ioctl );

	SMDEV_TEST_RUN(
	 "libsmdev_optical_disc_get_table_of_contents_scsi",
	 smdev_test_optical_disc_get_table_of_contents_scsi );

#endif /* defined( HAVE_LINUX_CDROM_H ) */

#endif /* defined( __GNUC__ ) && !defined( LIBSMDEV_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

