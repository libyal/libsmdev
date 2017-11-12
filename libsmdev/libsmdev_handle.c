/*
 * Handle functions
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_SYS_STAT_H )
#include <sys/stat.h>
#endif

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( WINAPI )
#include <winioctl.h>

#elif defined( HAVE_CYGWIN_FS_H )
#include <cygwin/fs.h>

#elif defined( HAVE_LINUX_FS_H )
/* Required for Linux platforms that use a sizeof( u64 )
 * in linux/fs.h but have no typedef of it
 */
#if !defined( HAVE_U64 )
typedef size_t u64;
#endif

#include <linux/fs.h>

#else

#if defined( HAVE_SYS_DISK_H )
#include <sys/disk.h>
#endif

#if defined( HAVE_SYS_DISKLABEL_H )
#include <sys/disklabel.h>
#endif

#endif /* defined( WINAPI ) */

#include "libsmdev_ata.h"
#include "libsmdev_definitions.h"
#include "libsmdev_handle.h"
#include "libsmdev_libcdata.h"
#include "libsmdev_libcerror.h"
#include "libsmdev_libcfile.h"
#include "libsmdev_libclocale.h"
#include "libsmdev_libcnotify.h"
#include "libsmdev_libuna.h"
#include "libsmdev_optical_disc.h"
#include "libsmdev_scsi.h"
#include "libsmdev_sector_range.h"
#include "libsmdev_string.h"
#include "libsmdev_track_value.h"
#include "libsmdev_types.h"

#if defined( WINAPI )

#if !defined( IOCTL_STORAGE_QUERY_PROPERTY )
#define IOCTL_STORAGE_QUERY_PROPERTY \
	CTL_CODE( IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS )

typedef enum _STORAGE_PROPERTY_ID
{
	StorageDeviceProperty = 0,
	StorageAdapterProperty,
	StorageDeviceIdProperty,
	StorageDeviceUniqueIdProperty,
	StorageDeviceWriteCacheProperty,
	StorageMiniportProperty,
	StorageAccessAlignmentProperty,
	StorageDeviceSeekPenaltyProperty,
	StorageDeviceTrimProperty,
	StorageDeviceWriteAggregationProperty
}
STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

typedef enum _STORAGE_QUERY_TYPE
{
	PropertyStandardQuery = 0,
	PropertyExistsQuery,
	PropertyMaskQuery,
	PropertyQueryMaxDefined
}
STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;

#if defined( _MSC_VER ) || defined( __BORLANDC__ )
#define HAVE_WINIOCTL_H_STORAGE_BUS_TYPE
#endif

#if !defined( HAVE_WINIOCTL_H_STORAGE_BUS_TYPE )

typedef enum _STORAGE_BUS_TYPE
{
	BusTypeUnknown		= 0x00,
	BusTypeScsi		= 0x01,
	BusTypeAtapi		= 0x02,
	BusTypeAta		= 0x03,
	BusType1394		= 0x04,
	BusTypeSsa		= 0x05,
	BusTypeFibre		= 0x06,
	BusTypeUsb		= 0x07,
	BusTypeRAID		= 0x08,
	BusTypeiSCSI		= 0x09,
	BusTypeSas		= 0x0a,
	BusTypeSata		= 0x0b,
	BusTypeMaxReserved	= 0x7f
}
STORAGE_BUS_TYPE, *PSTORAGE_BUS_TYPE;

#endif /* !defined( HAVE_WINIOCTL_H_STORAGE_BUS_TYPE ) */

typedef struct _STORAGE_PROPERTY_QUERY
{
	STORAGE_PROPERTY_ID PropertyId;
	STORAGE_QUERY_TYPE QueryType;
	UCHAR AdditionalParameters[ 1 ];
}
STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;

typedef struct _STORAGE_DEVICE_DESCRIPTOR
{
	ULONG Version;
	ULONG Size;
	UCHAR DeviceType;
	UCHAR DeviceTypeModifier;
	BOOLEAN RemovableMedia;
	BOOLEAN CommandQueueing;
	ULONG VendorIdOffset;
	ULONG ProductIdOffset;
	ULONG ProductRevisionOffset;
	ULONG SerialNumberOffset;
	STORAGE_BUS_TYPE BusType;
	ULONG RawPropertiesLength;
	UCHAR RawDeviceProperties[ 1 ];
}
STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;

typedef struct _STORAGE_DESCRIPTOR_HEADER
{
	ULONG Version;
	ULONG Size;
}
STORAGE_DESCRIPTOR_HEADER, *PSTORAGE_DESCRIPTOR_HEADER;

#endif /* !defined( IOCTL_STORAGE_QUERY_PROPERTY ) */

#if !defined( IOCTL_DISK_GET_LENGTH_INFO )
#define IOCTL_DISK_GET_LENGTH_INFO \
	CTL_CODE( IOCTL_DISK_BASE, 0x0017, METHOD_BUFFERED, FILE_READ_ACCESS )

typedef struct
{
	LARGE_INTEGER Length;
}
GET_LENGTH_INFORMATION;

#endif /* !defined( IOCTL_DISK_GET_LENGTH_INFO ) */

#if !defined( IOCTL_DISK_GET_DRIVE_GEOMETRY_EX )
#define IOCTL_DISK_GET_DRIVE_GEOMETRY_EX \
	CTL_CODE( IOCTL_DISK_BASE, 0x0028, METHOD_BUFFERED, FILE_ANY_ACCESS )

typedef struct _DISK_GEOMETRY_EX
{
	DISK_GEOMETRY Geometry;
	LARGE_INTEGER DiskSize;
	UCHAR Data[ 1 ];
}
DISK_GEOMETRY_EX, *PDISK_GEOMETRY_EX;

#endif /* !defined( IOCTL_DISK_GET_DRIVE_GEOMETRY_EX ) */

#endif /* defined( WINAPI ) */

/* Creates a handle
 * Make sure the value handle is referencing, is set to NULL
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
	if( libcdata_array_initialize(
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
	if( libcdata_array_initialize(
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
	if( libcdata_array_initialize(
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
	if( libcdata_range_list_initialize(
	     &( internal_handle->errors_range_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create errors range list.",
		 function );

		goto on_error;
	}
	internal_handle->number_of_error_retries = 2;

	*handle = (libsmdev_handle_t *) internal_handle;

	return( 1 );

on_error:
	if( internal_handle != NULL )
	{
		if( internal_handle->lead_outs_array != NULL )
		{
			libcdata_array_free(
			 &( internal_handle->lead_outs_array ),
			 NULL,
			 NULL );
		}
		if( internal_handle->sessions_array != NULL )
		{
			libcdata_array_free(
			 &( internal_handle->sessions_array ),
			 NULL,
			 NULL );
		}
		if( internal_handle->tracks_array != NULL )
		{
			libcdata_array_free(
			 &( internal_handle->tracks_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 internal_handle );
	}
	return( -1 );
}

/* Frees a handle
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

		if( internal_handle->device_file != NULL )
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
		*handle = NULL;

		if( libcdata_array_free(
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
		if( libcdata_array_free(
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
		if( libcdata_array_free(
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
		if( libcdata_range_list_free(
		     &( internal_handle->errors_range_list ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free errors range list.",
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
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_open";
	size64_t media_size                         = 0;
	size_t filename_length                      = 0;
	uint32_t bytes_per_sector                   = 0;
	int result                                  = 0;

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

	if( internal_handle->device_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - device file value already set.",
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
	if( libcdata_array_empty(
	     internal_handle->tracks_array,
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
	if( libcdata_array_empty(
	     internal_handle->sessions_array,
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
	if( libcdata_array_empty(
	     internal_handle->lead_outs_array,
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
	if( libcdata_range_list_empty(
	     internal_handle->errors_range_list,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty errors range list.",
		 function );

		goto on_error;
	}
	filename_length = narrow_string_length(
	                   filename );

	if( libsmdev_handle_set_filename(
	     handle,
	     filename,
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
	if( libcfile_file_initialize(
	     &( internal_handle->device_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create device file.",
		 function );

		goto on_error;
	}
	if( libcfile_file_open(
	     internal_handle->device_file,
	     filename,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open device file.",
		 function );

		goto on_error;
	}
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
	/* /dev/rdisk# on Mac OS X and some Windows devices require
	 * sector aligned read and seek operations
	 */
	result = libsmdev_handle_get_bytes_per_sector(
	          handle,
	          &bytes_per_sector,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libcfile_file_set_block_size(
		     internal_handle->device_file,
		     (size_t) bytes_per_sector,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to set block size in device file.",
			 function );

			goto on_error;
		}
	}
	/* Use this function to double the read-ahead system buffer on POSIX system
	 * This provides for some additional performance
	 */
	if( libcfile_file_set_access_behavior(
	     internal_handle->device_file,
	     LIBCFILE_ACCESS_BEHAVIOR_SEQUENTIAL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set access behavior.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_handle->device_file != NULL )
	{
		libcfile_file_free(
		 &( internal_handle->device_file ),
		 NULL );
	}
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
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_open_wide";
	size64_t media_size                         = 0;
	size_t filename_length                      = 0;
	uint32_t bytes_per_sector                   = 0;
	int result                                  = 0;

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

	if( internal_handle->device_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - device file value already set.",
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
	if( libcdata_array_empty(
	     internal_handle->tracks_array,
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
	if( libcdata_array_empty(
	     internal_handle->sessions_array,
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
	if( libcdata_array_empty(
	     internal_handle->lead_outs_array,
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
	if( libcdata_range_list_empty(
	     internal_handle->errors_range_list,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty errors range list.",
		 function );

		goto on_error;
	}
	filename_length = wide_string_length(
	                   filename );

	if( libsmdev_handle_set_filename_wide(
	     handle,
	     filename,
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
	if( libcfile_file_initialize(
	     &( internal_handle->device_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create device file.",
		 function );

		goto on_error;
	}
	if( libcfile_file_open_wide(
	     internal_handle->device_file,
	     filename,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open device file.",
		 function );

		goto on_error;
	}
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
	/* /dev/rdisk# on Mac OS X and some Windows devices require
	 * sector aligned read and seek operations
	 */
	result = libsmdev_handle_get_bytes_per_sector(
	          handle,
	          &bytes_per_sector,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bytes per sector.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libcfile_file_set_block_size(
		     internal_handle->device_file,
		     (size_t) bytes_per_sector,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to set block size in device file.",
			 function );

			goto on_error;
		}
	}
	/* Use this function to double the read-ahead system buffer on POSIX system
	 * This provides for some additional performance
	 */
	if( libcfile_file_set_access_behavior(
	     internal_handle->device_file,
	     LIBCFILE_ACCESS_BEHAVIOR_SEQUENTIAL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set access behavior.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_handle->device_file != NULL )
	{
		libcfile_file_free(
		 &( internal_handle->device_file ),
		 NULL );
	}
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

	if( internal_handle->device_file != NULL )
	{
		if( libcfile_file_close(
		     internal_handle->device_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close device file.",
			 function );

			result = -1;
		}
		if( libcfile_file_free(
		     &( internal_handle->device_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free device file.",
			 function );

			result = -1;
		}
	}
	internal_handle->offset                = 0;
	internal_handle->bytes_per_sector      = 0;
	internal_handle->bytes_per_sector_set  = 0;
	internal_handle->media_size            = 0;
	internal_handle->media_size_set        = 0;
	internal_handle->bus_type              = 0;
	internal_handle->device_type           = 0;
	internal_handle->removable             = 0;
	internal_handle->media_information_set = 0;

	if( memory_set(
	     internal_handle->vendor,
	     0,
	     64 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear vendor string.",
		 function );

		result = -1;
	}
	if( memory_set(
	     internal_handle->model,
	     0,
	     64 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear model string.",
		 function );

		result = -1;
	}
	if( memory_set(
	     internal_handle->serial_number,
	     0,
	     64 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear serial number string.",
		 function );

		result = -1;
	}
	if( internal_handle->filename != NULL )
	{
		memory_free(
		 internal_handle->filename );

		internal_handle->filename = NULL;
	}
	if( libcdata_array_empty(
	     internal_handle->tracks_array,
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
	if( libcdata_array_empty(
	     internal_handle->sessions_array,
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
	if( libcdata_array_empty(
	     internal_handle->lead_outs_array,
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
	if( libcdata_range_list_empty(
	     internal_handle->errors_range_list,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty errors range list.",
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
         uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
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
	uint32_t error_code                         = 0;
	int16_t number_of_read_errors               = 0;

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

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing device file.",
		 function );

		return( -1 );
	}
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
		read_count = libcfile_file_read_buffer_with_error_code(
			      internal_handle->device_file,
			      &( buffer[ buffer_offset ] ),
			      read_size,
			      &error_code,
		              error );

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
		if( read_count == -1 )
		{
			switch( error_code )
			{
#if defined( WINAPI )
				default:
#else
				/* Reading should not be retried for some POSIX error conditions
				 */
				case ESPIPE:
				case EPERM:
				case ENXIO:
				case ENODEV:
#endif
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_OPEN_FAILED,
					 "%s: unable to read from device file.",
					 function );

					return( -1 );

#if defined( WINAPI )
				/* A WINAPI read error generates the error code ERROR_UNRECOGNIZED_MEDIA
				 */
				case ERROR_UNRECOGNIZED_MEDIA:
#else
				default:
#endif
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: error reading from device file.\n",
						 function );

						if( ( error != NULL )
						 && ( *error != NULL ) )
						{
					                libcnotify_print_error_backtrace(
					                 *error );
						}
					}
#endif
                			libcerror_error_free(
		        	         error );

					current_offset = libcfile_file_seek_offset(
							  internal_handle->device_file,
							  0,
							  SEEK_CUR,
							  error );

					if( current_offset == -1 )
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

					/* On MacOS-X the read count can be -1 on error and the file offset is set to the position of the error
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
					break;

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
		else if( read_count == 0 )
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
		/* Not all requested data was read or there was an error
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
				     &( buffer[ error_granularity_buffer_offset ] ),
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
				     &( buffer[ buffer_offset ] ),
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
			if( libcdata_range_list_insert_range(
			     internal_handle->errors_range_list,
			     (uint64_t) current_offset,
			     (uint64_t) read_error_size,
			     NULL,
			     NULL,
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to insert read error to range list.",
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
			if( libcfile_file_seek_offset(
			     internal_handle->device_file,
			     error_granularity_skip_size,
			     SEEK_CUR,
			     error ) == -1 )
			{
				libcerror_system_set_error(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_SEEK_FAILED,
				 errno,
				 "%s: unable to skip %" PRIu32 " bytes after read error.",
				 function,
				 error_granularity_skip_size );

				return( -1 );
			}
			read_size            -= error_granularity_skip_size;
			buffer_offset        += error_granularity_skip_size;
			number_of_read_errors = 0;
		}
	}
	internal_handle->offset += buffer_offset;

	return( (ssize_t) buffer_offset );
}

/* Reads a buffer at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libsmdev_handle_read_buffer_at_offset(
         libsmdev_handle_t *handle,
         uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libsmdev_handle_read_buffer_at_offset";
	ssize_t read_count    = 0;

	if( libsmdev_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libsmdev_handle_read_buffer(
	              handle,
	              buffer,
	              buffer_size,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Writes a buffer
 * Returns the number of bytes written or -1 on error
 */
ssize_t libsmdev_handle_write_buffer(
         libsmdev_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_write_buffer";
	ssize_t write_count                         = 0;

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

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing device file.",
		 function );

		return( -1 );
	}
	write_count = libcfile_file_write_buffer(
	               internal_handle->device_file,
	               buffer,
	               buffer_size,
	               error );

	if( write_count < 0 )
	{
		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 errno,
		 "%s: unable to write to device file.",
		 function );

		return( -1 );
	}
	internal_handle->offset += write_count;

	return( write_count );
}

/* Writes a buffer at a specific offset
 * Returns the number of bytes written or -1 on error
 */
ssize_t libsmdev_handle_write_buffer_at_offset(
         libsmdev_handle_t *handle,
         const uint8_t *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libsmdev_handle_write_buffer_at_offset";
	ssize_t write_count   = 0;

	if( libsmdev_handle_seek_offset(
	     handle,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	write_count = libsmdev_handle_write_buffer(
	               handle,
	               buffer,
	               buffer_size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

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

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing device file.",
		 function );

		return( -1 );
	}
	offset = libcfile_file_seek_offset(
	          internal_handle->device_file,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_system_set_error(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 errno,
		 "%s: unable to seek offset in device file.",
		 function );

		return( -1 );
	}
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

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	if( system_string_copy(
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
	static char *function                       = "libsmdev_handle_set_filename";

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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

	if( internal_handle->device_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - device file value already set.",
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
		memory_free(
		 internal_handle->filename );

		internal_handle->filename      = NULL;
		internal_handle->filename_size = 0;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	internal_handle->filename = system_string_allocate(
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
	if( system_string_copy(
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

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
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

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
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

	if( internal_handle->device_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle - device file value already set.",
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
		memory_free(
		 internal_handle->filename );

		 internal_handle->filename      = NULL;
		 internal_handle->filename_size = 0;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
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
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	internal_handle->filename = system_string_allocate(
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
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
#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Determines the media information
 * Returns 1 if successful, 0 if no media information available or -1 on error
 */
int libsmdev_internal_handle_determine_media_information(
     libsmdev_internal_handle_t *internal_handle,
     libcerror_error_t **error )
{
#if defined( WINAPI )
	STORAGE_PROPERTY_QUERY query;
#elif defined( HDIO_GET_IDENTITY )
	struct hd_driveid device_configuration;
#endif
	uint8_t *response      = NULL;
	static char *function  = "libsmdev_internal_handle_determine_media_information";

#if defined( WINAPI )
	size_t response_size   = 1024;
	size_t string_length   = 0;
	ssize_t read_count     = 0;

#else
	size_t response_size   = 255;

#if defined( HAVE_SCSI_SG_H )
	ssize_t response_count = 0;
#endif
#endif
#if defined( HAVE_SCSI_SG_H ) || defined( HDIO_GET_IDENTITY ) || defined( HAVE_LINUX_CDROM_H ) || defined( WINAPI )
	ssize_t result         = 0;
#endif

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device handle.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_information_set != 0 )
	{
		return( 1 );
	}
	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid device handle - missing device file.",
		 function );

		return( -1 );
	}
	response = (uint8_t *) memory_allocate(
	                        sizeof( uint8_t ) * response_size );

	if( response == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to response.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     response,
	     0,
	     sizeof( uint8_t ) * response_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear response.",
		 function );

		goto on_error;
	}
#if defined( WINAPI )
	if( memset(
	     &query,
	     0,
	     sizeof( STORAGE_PROPERTY_QUERY ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear storage property query.",
		 function );

		goto on_error;
	}
	query.PropertyId = StorageDeviceProperty;
	query.QueryType  = PropertyStandardQuery;

	read_count = libcfile_file_io_control_read(
	              internal_handle->device_file,
	              IOCTL_STORAGE_QUERY_PROPERTY,
	              (uint8_t *) &query,
	              sizeof( STORAGE_PROPERTY_QUERY ),
	              response,
	              response_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_IOCTL_FAILED,
		 "%s: unable to query device for: IOCTL_STORAGE_QUERY_PROPERTY.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( ( error != NULL )
			 && ( *error != NULL ) )
			{
				libcnotify_print_error_backtrace(
				 *error );
			}
		}
#endif
		libcerror_error_free(
		 error );

		return( 0 );
	}
	if( (size_t) ( (STORAGE_DESCRIPTOR_HEADER *) response )->Size > response_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: response buffer too small.\n",
		 function );

		goto on_error;
	}
	if( (size_t) ( (STORAGE_DESCRIPTOR_HEADER *) response )->Size > sizeof( STORAGE_DEVICE_DESCRIPTOR ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_print_data(
			 response,
			 (size_t) ( (STORAGE_DESCRIPTOR_HEADER *) response )->Size,
			 0 );
		}
#endif
		if( ( (STORAGE_DEVICE_DESCRIPTOR *) response )->VendorIdOffset > 0 )
		{
			string_length = narrow_string_length(
					 (char *) &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->VendorIdOffset ] ) );

			result = libsmdev_string_trim_copy_from_byte_stream(
				  internal_handle->vendor,
				  64,
				  &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->VendorIdOffset ] ),
				  string_length,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set vendor.",
				 function );

				goto on_error;
			}
		}
		if( ( (STORAGE_DEVICE_DESCRIPTOR *) response )->ProductIdOffset > 0 )
		{
			string_length = narrow_string_length(
					 (char *) &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->ProductIdOffset ] ) );

			result = libsmdev_string_trim_copy_from_byte_stream(
				  internal_handle->model,
				  64,
				  &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->ProductIdOffset ] ),
				  string_length,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set model.",
				 function );

				goto on_error;
			}
		}
		if( ( (STORAGE_DEVICE_DESCRIPTOR *) response )->SerialNumberOffset > 0 )
		{
			string_length = narrow_string_length(
					 (char *) &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->SerialNumberOffset ] ) );

			result = libsmdev_string_trim_copy_from_byte_stream(
				  internal_handle->serial_number,
				  64,
				  &( response[ ( (STORAGE_DEVICE_DESCRIPTOR *) response )->SerialNumberOffset ] ),
				  string_length,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set serial number.",
				 function );

				goto on_error;
			}
		}
		internal_handle->removable = ( (STORAGE_DEVICE_DESCRIPTOR *) response )->RemovableMedia;

		switch( ( ( STORAGE_DEVICE_DESCRIPTOR *) response )->BusType )
		{
			case BusTypeScsi:
				internal_handle->bus_type = LIBSMDEV_BUS_TYPE_SCSI;
				break;

			case BusTypeAtapi:
			case BusTypeAta:
				internal_handle->bus_type = LIBSMDEV_BUS_TYPE_ATA;
				break;

			case BusType1394:
				internal_handle->bus_type = LIBSMDEV_BUS_TYPE_FIREWIRE;
				break;

			case BusTypeUsb:
				internal_handle->bus_type = LIBSMDEV_BUS_TYPE_USB;
				break;

			default:
				break;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Bus type:\t\t" );

			switch( ( ( STORAGE_DEVICE_DESCRIPTOR *) response )->BusType )
			{
				case BusTypeScsi:
					libcnotify_printf(
					 "SCSI" );
					break;

				case BusTypeAtapi:
					libcnotify_printf(
					 "ATAPI" );
					break;

				case BusTypeAta:
					libcnotify_printf(
					 "ATA" );
					break;

				case BusType1394:
					libcnotify_printf(
					 "FireWire (IEEE1394)" );
					break;

				case BusTypeSsa:
					libcnotify_printf(
					 "Serial Storage Architecture (SSA)" );
					break;

				case BusTypeFibre:
					libcnotify_printf(
					 "Fibre Channel" );
					break;

				case BusTypeUsb:
					libcnotify_printf(
					 "USB" );
					break;

				case BusTypeRAID:
					libcnotify_printf(
					 "RAID" );
					break;

				case BusTypeiScsi:
					libcnotify_printf(
					 "iSCSI" );
					break;

				case BusTypeSas:
					libcnotify_printf(
					 "SAS" );
					break;

				case BusTypeSata:
					libcnotify_printf(
					 "SATA" );
					break;

				case BusTypeSd:
					libcnotify_printf(
					 "Secure Digital (SD)" );
					break;

				case BusTypeMmc:
					libcnotify_printf(
					 "Multi Media Card (MMC)" );
					break;

				default:
					libcnotify_printf(
					 "Unknown: %d",
					 ( ( STORAGE_DEVICE_DESCRIPTOR *) response )->BusType );
					break;
			}
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
#else
#if defined( HAVE_SCSI_SG_H )
	/* Use the Linux sg (generic SCSI) driver to determine device information
	 */
	result = libsmdev_scsi_get_bus_type(
	          internal_handle->device_file,
	          &( internal_handle->bus_type ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine bus type.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		response_count = libsmdev_scsi_inquiry(
				  internal_handle->device_file,
				  0x00,
				  0x00,
				  response,
				  response_size,
				  error );

		if( response_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: SCSI Inquiry.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
/* TODO handle garbarge return ? */
		if( response_count >= 5 )
		{
			internal_handle->removable   = ( response[ 1 ] & 0x80 ) >> 7;
			internal_handle->device_type = ( response[ 0 ] & 0x1f );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: removable\t\t: %" PRIu8 "\n",
				 function,
				 internal_handle->removable );

				libcnotify_printf(
				 "%s: device type\t: 0x%" PRIx8 "\n",
				 function,
				 internal_handle->device_type );

				libcnotify_printf(
				 "\n" );
			}
#endif
		}
		if( response_count >= 16 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_print_data(
				 response,
				 response_count,
				 0 );
			}
#endif
			result = libsmdev_string_trim_copy_from_byte_stream(
				  internal_handle->vendor,
				  64,
				  &( response[ 8 ] ),
				  15 - 8,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set vendor.",
				 function );

				goto on_error;
			}
		}
		if( response_count >= 32 )
		{
			result = libsmdev_string_trim_copy_from_byte_stream(
				  internal_handle->model,
				  64,
				  &( response[ 16 ] ),
				  31 - 16,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set model.",
				 function );

				goto on_error;
			}
		}
		response_count = libsmdev_scsi_inquiry(
				  internal_handle->device_file,
				  0x01,
				  0x80,
				  response,
				  response_size,
				  error );

		if( response_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: SCSI Inquiry.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
		if( response_count > 4 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_print_data(
				 response,
				 response_count,
				 0 );
			}
#endif
			result = libsmdev_string_trim_copy_from_byte_stream(
				  internal_handle->serial_number,
				  64,
				  &( response[ 4 ] ),
				  response_count - 4,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set serial number.",
				 function );

				goto on_error;
			}
		}
	}
#endif
#if defined( HDIO_GET_IDENTITY )
	if( internal_handle->bus_type == LIBSMDEV_BUS_TYPE_ATA )
	{
		result = libsmdev_ata_get_device_configuration(
		          internal_handle->device_file,
		          &device_configuration,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve ATA device configuration.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			result = libsmdev_string_trim_copy_from_byte_stream(
				  internal_handle->serial_number,
				  64,
				  device_configuration.serial_no,
				  20,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set serial number.",
				 function );

				goto on_error;
			}
			result = libsmdev_string_trim_copy_from_byte_stream(
				  internal_handle->model,
				  64,
				  device_configuration.model,
				  40,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set model.",
				 function );

				goto on_error;
			}
			internal_handle->removable   = ( device_configuration.config & 0x0080 ) >> 7;
			internal_handle->device_type = ( device_configuration.config & 0x1f00 ) >> 8;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: removable\t\t: %" PRIu8 "\n",
				 function,
				 internal_handle->removable );

				libcnotify_printf(
				 "%s: device type\t: 0x%" PRIx8 "\n",
				 function,
				 internal_handle->device_type );

				libcnotify_printf(
				 "\n" );
			}
#endif
		}
	}
#endif
#if defined( HAVE_LINUX_CDROM_H )
	if( internal_handle->device_type == 0x05 )
	{
		result = libsmdev_optical_disc_get_table_of_contents(
		          internal_handle->device_file,
		          internal_handle,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve optical disc table of contents.",
			 function );

			goto on_error;
		}
	}
#endif
/* Disabled for now
	if( libsmdev_scsi_get_identier(
	     internal_handle->device_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine SCSI identifier.",
		 function );

		goto on_error;
	}
	uint8_t pci_bus_address[ 64 ];
	size_t pci_bus_address_size = 64;

	if( libsmdev_scsi_get_pci_bus_address(
	     internal_handle->device_file,
	     pci_bus_address,
	     pci_bus_address_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine PCI bus address.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LINUX_USB_CH9_H )
	if( internal_handle->bus_type == LIBSMDEV_BUS_TYPE_USB )
	{
		if( io_usb_test(
		     internal_handle->device_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to test USB.",
			 function );

			goto on_error;
		}
	}
#endif
*/
#endif
	internal_handle->media_information_set = 1;

	memory_free(
	 response );

	response = NULL;

	return( 1 );

on_error:
	if( response != NULL )
	{
		memory_free(
		 response );
	}
	return( -1 );
}

/* Appends a session
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_append_session(
     libsmdev_internal_handle_t *internal_handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libsmdev_sector_range_t *sector_range = NULL;
	static char *function                 = "libsmdev_handle_append_session";
	int entry_index                       = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libsmdev_sector_range_initialize(
	     &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector range.",
		 function );

		goto on_error;
	}
	if( libsmdev_sector_range_set(
	     sector_range,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sector range.",
		 function );

		goto on_error;
	}
	if( libcdata_array_append_entry(
	     internal_handle->sessions_array,
	     &entry_index,
	     (intptr_t *) sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append session sector range to array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sector_range != NULL )
	{
		libsmdev_sector_range_free(
		 &sector_range,
		 NULL );
	}
	return( -1 );
}

/* Appends a lead-out
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_append_lead_out(
     libsmdev_internal_handle_t *internal_handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     libcerror_error_t **error )
{
	libsmdev_sector_range_t *sector_range = NULL;
	static char *function                 = "libsmdev_handle_append_lead_out";
	int entry_index                       = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libsmdev_sector_range_initialize(
	     &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector range.",
		 function );

		goto on_error;
	}
	if( libsmdev_sector_range_set(
	     sector_range,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sector range.",
		 function );

		goto on_error;
	}
	if( libcdata_array_append_entry(
	     internal_handle->lead_outs_array,
	     &entry_index,
	     (intptr_t *) sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append lead-out sector range to array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sector_range != NULL )
	{
		libsmdev_sector_range_free(
		 &sector_range,
		 NULL );
	}
	return( -1 );
}

/* Appends a track
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_append_track(
     libsmdev_internal_handle_t *internal_handle,
     uint64_t start_sector,
     uint64_t number_of_sectors,
     uint8_t type,
     libcerror_error_t **error )
{
	libsmdev_track_value_t *track_value = NULL;
	static char *function               = "libsmdev_handle_append_track";
	int entry_index                     = 0;

	if( internal_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( libsmdev_track_value_initialize(
	     &track_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create track value.",
		 function );

		goto on_error;
	}
	if( libsmdev_track_value_set(
	     track_value,
	     start_sector,
	     number_of_sectors,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set track value.",
		 function );

		goto on_error;
	}
	if( libcdata_array_append_entry(
	     internal_handle->tracks_array,
	     &entry_index,
	     (intptr_t *) track_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append track to array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( track_value != NULL )
	{
		libsmdev_track_value_free(
		 &track_value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the media size
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_media_size(
     libsmdev_handle_t *handle,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_media_size";

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

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid handle - missing device file.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_size_set == 0 )
	{
		if( libcfile_file_get_size(
		     internal_handle->device_file,
		     &( internal_handle->media_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine device file size.",
			 function );

			return( -1 );
		}
		internal_handle->media_size_set = 1;
	}
	*media_size = internal_handle->media_size;

	return( 1 );
}

/* Retrieves the number of bytes per sector
 * Returns the 1 if succesful, 0 if not or -1 on error
 */
int libsmdev_handle_get_bytes_per_sector(
     libsmdev_handle_t *handle,
     uint32_t *bytes_per_sector,
     libcerror_error_t **error )
{
#if defined( WINAPI )
	DISK_GEOMETRY disk_geometry;
	DISK_GEOMETRY_EX disk_geometry_extended;
#endif

	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_bytes_per_sector";
	ssize_t read_count                          = 0;

#if defined( WINAPI )
	uint32_t error_code                         = 0;

#elif !defined( BLKSSZGET ) && defined( DIOCGSECTORSIZE )
	u_int safe_bytes_per_sector                 = 0;
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

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid device handle - missing device file.",
		 function );

		return( -1 );
	}
	if( bytes_per_sector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bytes per sector.",
		 function );

		return( -1 );
	}
	if( internal_handle->bytes_per_sector_set == 0 )
	{
#if defined( WINAPI )
		read_count = libcfile_file_io_control_read_with_error_code(
		              internal_handle->device_file,
		              IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
		              NULL,
		              0,
		              (uint8_t *) &disk_geometry_extended,
		              sizeof( DISK_GEOMETRY_EX ),
		              &error_code,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: IOCTL_DISK_GET_DRIVE_GEOMETRY_EX.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );

			if( error_code == ERROR_NOT_SUPPORTED )
			{
				/* A floppy device does not support IOCTL_DISK_GET_DRIVE_GEOMETRY_EX
				 */
				read_count = libcfile_file_io_control_read(
				              internal_handle->device_file,
				              IOCTL_DISK_GET_DRIVE_GEOMETRY,
				              NULL,
				              0,
				              (uint8_t *) &disk_geometry,
				              sizeof( DISK_GEOMETRY ),
				              error );

				if( read_count == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_IOCTL_FAILED,
					 "%s: unable to query device for: IOCTL_DISK_GET_DRIVE_GEOMETRY.",
					 function );

#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( ( error != NULL )
						 && ( *error != NULL ) )
						{
							libcnotify_print_error_backtrace(
							 *error );
						}
					}
#endif
					libcerror_error_free(
					 error );
				}
				else
				{
					internal_handle->bytes_per_sector     = (uint32_t) disk_geometry.BytesPerSector;
					internal_handle->bytes_per_sector_set = 1;
				}
			}
		}
		else
		{
			internal_handle->bytes_per_sector     = (uint32_t) disk_geometry_extended.Geometry.BytesPerSector;
			internal_handle->bytes_per_sector_set = 1;
		}
#elif defined( BLKSSZGET )
		read_count = libcfile_file_io_control_read(
		              internal_handle->device_file,
		              BLKSSZGET,
		              NULL,
		              0,
		              (uint8_t *) &( internal_handle->bytes_per_sector ),
		              4,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: BLKSSZGET.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
		else
		{
			internal_handle->bytes_per_sector_set = 1;
		}
#elif defined( DIOCGSECTORSIZE )
		read_count = libcfile_file_io_control_read(
		              internal_handle->device_file,
		              DIOCGSECTORSIZE,
		              NULL,
		              0,
		              (uint8_t *) &safe_bytes_per_sector,
		              sizeof( u_int ),
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: DIOCGSECTORSIZE.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
		else if( safe_bytes_per_sector > (u_int) UINT32_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid bytes per sector value out of bounds.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );

		}
		else
		{
			internal_handle->bytes_per_sector     = (uint32_t) safe_bytes_per_sector;
			internal_handle->bytes_per_sector_set = 1;
		}
#elif defined( DKIOCGETBLOCKSIZE )
		read_count = libcfile_file_io_control_read(
		              internal_handle->device_file,
		              DKIOCGETBLOCKSIZE,
		              NULL,
		              0,
		              (uint8_t *) &( internal_handle->bytes_per_sector ),
		              4,
		              error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_IOCTL_FAILED,
			 "%s: unable to query device for: DKIOCGETBLOCKSIZE.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );
		}
		else
		{
			internal_handle->bytes_per_sector_set = 1;
		}
#endif
	}
	if( internal_handle->bytes_per_sector_set == 0 )
	{
#if defined( BLKSSZGET ) || defined( DKIOCGETBLOCKSIZE ) || defined( DIOCGSECTORSIZE ) || defined( WINAPI )
		return( 0 );
#else
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported platform.",
		 function );

		return( -1 );
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: bytes per sector: %" PRIu32 "\n",
		 function,
		 internal_handle->bytes_per_sector );
	}
#endif
	*bytes_per_sector = internal_handle->bytes_per_sector;

	return( 1 );
}

/* Retrieves the media type
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_media_type(
     libsmdev_handle_t *handle,
     uint8_t *media_type,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_media_type";

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

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid device handle - missing device file.",
		 function );

		return( -1 );
	}
	if( media_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media type.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_information_set == 0 )
	{
		if( libsmdev_internal_handle_determine_media_information(
		     internal_handle,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media information.",
			 function );

			return( -1 );
		}
	}
	if( internal_handle->device_type == 0x05 )
	{
		*media_type = LIBSMDEV_MEDIA_TYPE_OPTICAL;
	}
	else if( internal_handle->removable != 0 )
	{
		*media_type = LIBSMDEV_MEDIA_TYPE_REMOVABLE;
	}
	else
	{
		*media_type = LIBSMDEV_MEDIA_TYPE_FIXED;
	}
	return( 1 );
}

/* Retrieves the bus type
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_bus_type(
     libsmdev_handle_t *handle,
     uint8_t *bus_type,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_bus_type";

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

	if( internal_handle->device_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid device handle - missing device file.",
		 function );

		return( -1 );
	}
	if( bus_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bus type.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_information_set == 0 )
	{
		if( libsmdev_internal_handle_determine_media_information(
		     internal_handle,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media information.",
			 function );

			return( -1 );
		}
	}
	*bus_type = internal_handle->bus_type;

	return( 1 );
}

/* Retrieves an UTF-8 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmdev_handle_get_utf8_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	uint8_t *information_value                  = NULL;
	static char *function                       = "libsmdev_handle_get_utf8_information_value";
	size_t information_value_size               = 0;
	size_t string_index                         = 0;

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

	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_information_set == 0 )
	{
		if( libsmdev_internal_handle_determine_media_information(
		     internal_handle,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media information.",
			 function );

			return( -1 );
		}
	}
	if( identifier_length == 5 )
	{
		if( narrow_string_compare(
		     "model",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->model;
		}
	}
	else if( identifier_length == 6 )
	{
		if( narrow_string_compare(
		     "vendor",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->vendor;
		}
	}
	else if( identifier_length == 13 )
	{
		if( narrow_string_compare(
		     "serial_number",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->serial_number;
		}
	}
	if( information_value == NULL )
	{
		return( 0 );
	}
	if( information_value[ 0 ] == 0 )
	{
		return( 0 );
	}
	/* Determine the header value size
	 */
	information_value_size = 1 + narrow_string_length(
	                              (char *) information_value );

	if( utf8_string_size < information_value_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string too small.",
		 function );

		return( -1 );
	}
	for( string_index = 0;
	     string_index < information_value_size - 1;
	     string_index++ )
	{
		utf8_string[ string_index ] = (uint8_t) information_value[ string_index ];
	}
	utf8_string[ information_value_size - 1 ] = 0;

	return( 1 );
}

/* Retrieves an UTF-16 encoded information value for the specific identifier
 * The value size should include the end of string character
 * Returns 1 if successful, 0 if value not present or -1 on error
 */
int libsmdev_handle_get_utf16_information_value(
     libsmdev_handle_t *handle,
     const uint8_t *identifier,
     size_t identifier_length,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	uint8_t *information_value                  = NULL;
	static char *function                       = "libsmdev_handle_get_utf16_information_value";
	size_t information_value_size               = 0;
	size_t string_index                         = 0;

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

	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( internal_handle->media_information_set == 0 )
	{
		if( libsmdev_internal_handle_determine_media_information(
		     internal_handle,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine media information.",
			 function );

			return( -1 );
		}
	}
	if( identifier_length == 5 )
	{
		if( narrow_string_compare(
		     "model",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->model;
		}
	}
	else if( identifier_length == 6 )
	{
		if( narrow_string_compare(
		     "vendor",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->vendor;
		}
	}
	else if( identifier_length == 13 )
	{
		if( narrow_string_compare(
		     "serial_number",
		     (char *) identifier,
		     identifier_length ) == 0 )
		{
			information_value = (uint8_t *) internal_handle->serial_number;
		}
	}
	if( information_value == NULL )
	{
		return( 0 );
	}
	if( information_value[ 0 ] == 0 )
	{
		return( 0 );
	}
	/* Determine the header value size
	 */
	information_value_size = 1 + narrow_string_length(
	                              (char *) information_value );

	if( utf16_string_size < information_value_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string too small.",
		 function );

		return( -1 );
	}
	for( string_index = 0;
	     string_index < information_value_size - 1;
	     string_index++ )
	{
		utf16_string[ string_index ] = (uint16_t) information_value[ string_index ];
	}
	utf16_string[ information_value_size - 1 ] = 0;

	return( 1 );
}

/* Retrieves the number of sessions
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_number_of_sessions(
     libsmdev_handle_t *handle,
     int *number_of_sessions,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_number_of_sessions";

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

	if( libcdata_array_get_number_of_entries(
	     internal_handle->sessions_array,
	     number_of_sessions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in sessions array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a session
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_session(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	libsmdev_sector_range_t *sector_range       = NULL;
	static char *function                       = "libsmdev_handle_get_session";

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

	if( libcdata_array_get_entry_by_index(
	     internal_handle->sessions_array,
	     index,
	     (intptr_t **) &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve session sector range: %d from array.",
		 function,
		 index );

		return( -1 );
	}
	if( libsmdev_sector_range_get(
	     sector_range,
	     start_sector,
	     number_of_sectors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sector range.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of tracks
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_number_of_tracks(
     libsmdev_handle_t *handle,
     int *number_of_tracks,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_number_of_tracks";

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

	if( libcdata_array_get_number_of_entries(
	     internal_handle->tracks_array,
	     number_of_tracks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries in tracks array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a track
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_track(
     libsmdev_handle_t *handle,
     int index,
     uint64_t *start_sector,
     uint64_t *number_of_sectors,
     uint8_t *type,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	libsmdev_track_value_t *track_value         = NULL;
	static char *function                       = "libsmdev_handle_get_track";

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

	if( libcdata_array_get_entry_by_index(
	     internal_handle->tracks_array,
	     index,
	     (intptr_t **) &track_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve track value: %d from array.",
		 function,
		 index );

		return( -1 );
	}
	if( libsmdev_track_value_get(
	     track_value,
	     start_sector,
	     number_of_sectors,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve track value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of read/write error retries
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t *number_of_error_retries,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_number_of_error_retries";

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

	if( number_of_error_retries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of error retries.",
		 function );

		return( -1 );
	}
	*number_of_error_retries = internal_handle->number_of_error_retries;

	return( 1 );
}

/* Sets the number of read/write error retries
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_set_number_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t number_of_error_retries,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_set_number_of_error_retries";

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

	internal_handle->number_of_error_retries = number_of_error_retries;

	return( 1 );
}

/* Retrieves the read/write error granularity
 * A value of 0 represents an error granularity of the entire buffer being read/written
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_error_granularity(
     libsmdev_handle_t *handle,
     size_t *error_granularity,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_error_granularity";

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

	if( error_granularity == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read granularity.",
		 function );

		return( -1 );
	}
	*error_granularity = internal_handle->error_granularity;

	return( 1 );
}

/* Sets the read/write error granularity
 * A value of 0 represents an error granularity of the entire buffer being read/written
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_set_error_granularity(
     libsmdev_handle_t *handle,
     size_t error_granularity,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_set_error_granularity";

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

	if( error_granularity > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid error granularity value exceeds maximum.",
		 function );

		return( -1 );
	}
	internal_handle->error_granularity = error_granularity;

	return( 1 );
}

/* Retrieves the read/write error flags
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_error_flags(
     libsmdev_handle_t *handle,
     uint8_t *error_flags,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_error_flags";

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

	if( error_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid error flags.",
		 function );

		return( -1 );
	}
	*error_flags = internal_handle->error_flags;

	return( 1 );
}

/* Sets the the read/write error flags
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_set_error_flags(
     libsmdev_handle_t *handle,
     uint8_t error_flags,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_set_error_flags";

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

	if( ( error_flags & ~( LIBSMDEV_ERROR_FLAG_ZERO_ON_ERROR ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported error flags.",
		 function );

		return( -1 );
	}
	internal_handle->error_flags = error_flags;

	return( 1 );
}

/* Retrieves the number of read/write errors
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_number_of_errors(
     libsmdev_handle_t *handle,
     int *number_of_errors,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_number_of_errors";

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

	if( libcdata_range_list_get_number_of_elements(
	     internal_handle->errors_range_list,
	     number_of_errors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements in errors range list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a read/write error
 * Returns 1 if successful or -1 on error
 */
int libsmdev_handle_get_error(
     libsmdev_handle_t *handle,
     int index,
     off64_t *offset,
     size64_t *size,
     libcerror_error_t **error )
{
	libsmdev_internal_handle_t *internal_handle = NULL;
	static char *function                       = "libsmdev_handle_get_error";
	intptr_t *value                             = NULL;

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

	if( libcdata_range_list_get_range_by_index(
	     internal_handle->errors_range_list,
	     index,
	     (uint64_t *) offset,
	     (uint64_t *) size,
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve error: %d from errors range list.",
		 function,
		 index );

		return( -1 );
	}
	return( 1 );
}


