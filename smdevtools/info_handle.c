/*
 * Info handle
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
#include <byte_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_SYS_UTSNAME_H )
#include <sys/utsname.h>
#endif

#include "byte_size_string.h"
#include "info_handle.h"
#include "smdevtools_libcerror.h"
#include "smdevtools_libsmdev.h"

#define INFO_HANDLE_VALUE_SIZE			512
#define INFO_HANDLE_VALUE_IDENTIFIER_SIZE	64
#define INFO_HANDLE_NOTIFY_STREAM		stdout

/* Retrieves the track type
 * Returns a string represenation of the track type
 */
const char *info_handle_get_track_type(
             uint8_t track_type )
{
	switch( track_type )
	{
		case LIBSMDEV_TRACK_TYPE_AUDIO:
			return( "audio (sector size: 2352)" );

		case LIBSMDEV_TRACK_TYPE_CDG:
			return( "CD+G (sector size: 2448)" );

		case LIBSMDEV_TRACK_TYPE_MODE1_2048:
			return( "mode1/2048 (sector size: 2048)" );

		case LIBSMDEV_TRACK_TYPE_MODE1_2352:
			return( "mode1/2352 (sector size: 2352)" );

		case LIBSMDEV_TRACK_TYPE_MODE2_2048:
			return( "mode2/2048 (sector size: 2048)" );

		case LIBSMDEV_TRACK_TYPE_MODE2_2324:
			return( "mode2/2324 (sector size: 2324)" );

		case LIBSMDEV_TRACK_TYPE_MODE2_2336:
			return( "mode2/2336 (sector size: 2336)" );

		case LIBSMDEV_TRACK_TYPE_MODE2_2352:
			return( "mode2/2352 (sector size: 2352)" );

		case LIBSMDEV_TRACK_TYPE_CDI_2336:
			return( "CDI/2336 (sector size: 2336)" );

		case LIBSMDEV_TRACK_TYPE_CDI_2352:
			return( "CDI/2352 (sector size: 2352)" );

		default:
			break;
	}
	return( "UNKNOWN" );
}

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
			info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		goto on_error;
	}
	if( libsmdev_handle_initialize(
	     &( ( *info_handle )->input_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input handle.",
		 function );

		goto on_error;
	}
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_handle != NULL )
		{
			if( libsmdev_handle_free(
			     &( ( *info_handle )->input_handle ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input handle.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle != NULL )
	{
		if( libsmdev_handle_signal_abort(
		     info_handle->input_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input handle to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Opens the input of the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "info_handle_open_input";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libsmdev_handle_open_wide(
	     info_handle->input_handle,
	     filename,
	     LIBSMDEV_OPEN_READ,
	     error ) != 1 )
#else
	if( libsmdev_handle_open(
	     info_handle->input_handle,
	     filename,
	     LIBSMDEV_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libsmdev_handle_close(
	     info_handle->input_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Prints the sessions
 * Returns 1 if successful or -1 on error
 */
int info_handle_sessions_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function      = "info_handle_sessions_fprint";
	uint64_t last_sector       = 0;
	uint64_t number_of_sectors = 0;
	uint64_t start_sector      = 0;
	int number_of_sessions     = 0;
	int result                 = 1;
	int session_index          = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libsmdev_handle_get_number_of_sessions(
	     info_handle->input_handle,
	     &number_of_sessions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sessions.",
		 function );

		return( -1 );
	}
	if( number_of_sessions > 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Sessions:\n" );
		fprintf(
		 info_handle->notify_stream,
		 "\ttotal number: %" PRIu32 "\n",
		 number_of_sessions );

		for( session_index = 0;
		     session_index < number_of_sessions;
		     session_index++ )
		{
			if( libsmdev_handle_get_session(
			     info_handle->input_handle,
			     session_index,
			     &start_sector,
			     &number_of_sectors,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the session: %d.",
				 function,
				 session_index );

				start_sector      = 0;
				number_of_sectors = 0;

				result = -1;
			}
			last_sector = start_sector + number_of_sectors;

			if( number_of_sectors != 0 )
			{
				last_sector -= 1;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\tat sectors(s): %" PRIu64 " - %" PRIu64 " number: %" PRIu64 "\n",
			 start_sector,
			 last_sector,
			 number_of_sectors );
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( result );
}

/* Prints the tracks
 * Returns 1 if successful or -1 on error
 */
int info_handle_tracks_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function           = "info_handle_tracks_fprint";
	uint64_t data_file_start_sector = 0;
	uint64_t last_sector            = 0;
	uint64_t number_of_sectors      = 0;
	uint64_t start_sector           = 0;
	uint8_t track_type              = 0;
	int data_file_index             = 0;
	int number_of_tracks            = 0;
	int result                      = 1;
	int track_index                 = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	if( libsmdev_handle_get_number_of_tracks(
	     info_handle->input_handle,
	     &number_of_tracks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of tracks.",
		 function );

		return( -1 );
	}
	if( number_of_tracks > 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "Tracks:\n" );
		fprintf(
		 info_handle->notify_stream,
		 "\ttotal number: %" PRIu32 "\n",
		 number_of_tracks );

		for( track_index = 0;
		     track_index < number_of_tracks;
		     track_index++ )
		{
			if( libsmdev_handle_get_track(
			     info_handle->input_handle,
			     track_index,
			     &start_sector,
			     &number_of_sectors,
			     &track_type,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the track: %d.",
				 function,
				 track_index );

				start_sector           = 0;
				number_of_sectors      = 0;
				track_type             = 0;
				data_file_index        = 0;
				data_file_start_sector = 0;

				result = -1;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\ttype: %s",
			 info_handle_get_track_type(
			  track_type ) );

			last_sector = start_sector + number_of_sectors;

			if( number_of_sectors != 0 )
			{
				last_sector -= 1;
			}
			fprintf(
			 info_handle->notify_stream,
			 " at sectors(s): %" PRIu64 " - %" PRIu64 " number: %" PRIu64 "",
			 start_sector,
			 last_sector,
			 number_of_sectors );

			fprintf(
			 info_handle->notify_stream,
			 " (data file: %d, start sector: %" PRIu64 ")",
			 data_file_index,
			 data_file_start_sector );

			fprintf(
			 info_handle->notify_stream,
			 "\n" );
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	return( result );
}

/* Prints the handle information
 * Returns 1 if successful or -1 on error
 */
int info_handle_handle_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	uint8_t media_information_value[ 64 ];
        system_character_t byte_size_string[ 16 ];

	static char *function     = "info_handle_handle_fprint";
	size64_t media_size       = 0;
	uint32_t bytes_per_sector = 0;
	uint8_t bus_type          = 0;
	uint8_t media_type        = 0;
	int result                = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Device information:\n" );

	if( libsmdev_handle_get_bus_type(
	     info_handle->input_handle,
	     &bus_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bus type.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tbus type\t\t: " );

	switch( bus_type )
	{
		case LIBSMDEV_BUS_TYPE_ATA:
			fprintf(
			 info_handle->notify_stream,
			 "ATA/ATAPI" );
			break;

		case LIBSMDEV_BUS_TYPE_FIREWIRE:
			fprintf(
			 info_handle->notify_stream,
			 "FireWire (IEEE1394)" );
			break;

		case LIBSMDEV_BUS_TYPE_SCSI:
			fprintf(
			 info_handle->notify_stream,
			 "SCSI" );
			break;

		case LIBSMDEV_BUS_TYPE_USB:
			fprintf(
			 info_handle->notify_stream,
			 "USB" );
			break;

		default:
			fprintf(
			 info_handle->notify_stream,
			 "Unknown: 0x%02" PRIx8 "",
			 bus_type );
			break;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	result = libsmdev_handle_get_utf8_information_value(
		  info_handle->input_handle,
		  (uint8_t *) "vendor",
		  6,
		  media_information_value,
		  64,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media information value: vendor.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		media_information_value[ 0 ] = 0;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tvendor\t\t\t: %s\n",
	 (char *) media_information_value );

	result = libsmdev_handle_get_utf8_information_value(
		  info_handle->input_handle,
		  (uint8_t *) "model",
		  5,
		  media_information_value,
		  64,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media information value: model.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		media_information_value[ 0 ] = 0;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tmodel\t\t\t: %s\n",
	 (char *) media_information_value );

	result = libsmdev_handle_get_utf8_information_value(
		  info_handle->input_handle,
		  (uint8_t *) "serial_number",
		  13,
		  media_information_value,
		  64,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media information value: serial_number.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		media_information_value[ 0 ] = 0;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tserial\t\t\t: %s\n",
	 (char *) media_information_value );

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "Storage media information:\n" );

	if( libsmdev_handle_get_media_type(
	     info_handle->input_handle,
	     &media_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media type.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tmedia type\t\t: " );

	switch( media_type )
	{
		case LIBSMDEV_MEDIA_TYPE_REMOVABLE:
			fprintf(
			 info_handle->notify_stream,
			 "Removable" );

			break;

		case LIBSMDEV_MEDIA_TYPE_FIXED:
			fprintf(
			 info_handle->notify_stream,
			 "Fixed" );

			break;

		case LIBSMDEV_MEDIA_TYPE_OPTICAL:
			fprintf(
			 info_handle->notify_stream,
			 "Optical" );

			break;

		case LIBSMDEV_MEDIA_TYPE_MEMORY:
			fprintf(
			 info_handle->notify_stream,
			 "Memory" );

			break;

		default:
			fprintf(
			 info_handle->notify_stream,
			 "unknown" );

			break;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libsmdev_handle_get_media_size(
	     info_handle->input_handle,
	     &media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		return( -1 );
	}
	result = byte_size_string_create(
	          byte_size_string,
	          16,
	          media_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	if( result == 1 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tmedia size\t\t: %" PRIs_SYSTEM " (%" PRIu64 " bytes)\n",
		 byte_size_string,
		 media_size );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tmedia size\t\t: %" PRIu64 " bytes\n",
		 media_size );
	}
	result = libsmdev_handle_get_bytes_per_sector(
	          info_handle->input_handle,
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

		return( -1 );
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tbytes per sector\t: %" PRIu32 "\n",
		 bytes_per_sector );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( info_handle_sessions_fprint(
	     info_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print sessions.",
		 function );

		return( -1 );
	}
	if( info_handle_tracks_fprint(
	     info_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print tracks.",
		 function );

		return( -1 );
	}
	return( 1 );
}

