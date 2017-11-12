/*
 * Shows information obtained from storage media (SM) devices
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
#include <memory.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#include "info_handle.h"
#include "smdevtools_getopt.h"
#include "smdevtools_libcerror.h"
#include "smdevtools_libclocale.h"
#include "smdevtools_libcnotify.h"
#include "smdevtools_libsmdev.h"
#include "smdevtools_output.h"
#include "smdevtools_signal.h"
#include "smdevtools_unused.h"

info_handle_t *smdevinfo_info_handle = NULL;
int smdevinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use smdevinfo to determine information about storage\n"
	                 "media (SM) device(s)\n\n" );

	fprintf( stream, "Usage: smdevinfo [ -hivV ] source\n\n" );

	fprintf( stream, "\tsource: the source device file\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-i:     ignore data file(s)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for smdevinfo
 */
void smdevinfo_signal_handler(
      smdevtools_signal_t signal SMDEVTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "smdevinfo_signal_handler";

	SMDEVTOOLS_UNREFERENCED_PARAMETER( signal )

	smdevinfo_abort = 1;

	if( ( smdevinfo_info_handle != NULL )
	 && ( info_handle_signal_abort(
	       smdevinfo_info_handle,
	       &error ) != 1 ) )
	{
		libcnotify_printf(
		 "%s: unable to signal info handle to abort.\n",
		 function );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error   = NULL;
	system_character_t *source = NULL;
	char *program              = "smdevinfo";
	system_integer_t option    = 0;
	uint8_t ignore_data_files  = 0;
	int verbose                = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "smdevtools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
        if( smdevtools_output_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	smdevoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = smdevtools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "ihvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'i':
				ignore_data_files = 1;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				smdevoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libsmdev_notify_set_stream(
	 stderr,
	 NULL );
	libsmdev_notify_set_verbose(
	 verbose );

	if( info_handle_initialize(
	     &smdevinfo_info_handle,
	     &error ) != 1 )
	{
		smdevoutput_version_fprint(
		 stderr,
		 program );

		fprintf(
		 stderr,
		 "Unable to create info handle.\n" );

		goto on_error;
	}
	smdevinfo_info_handle->ignore_data_files = ignore_data_files;

	if( smdevtools_signal_attach(
	     smdevinfo_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( info_handle_open_input(
	     smdevinfo_info_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open file: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	if( info_handle_handle_fprint(
	     smdevinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print information.\n" );

		goto on_error;
	}
	if( info_handle_close(
	     smdevinfo_info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( smdevtools_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( info_handle_free(
	     &smdevinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		goto on_error;
	}
	if( smdevinfo_abort != 0 )
	{
		fprintf(
		 stdout,
		 "%s: ABORTED\n",
		 program );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( smdevinfo_info_handle != NULL )
	{
		info_handle_free(
		 &smdevinfo_info_handle,
		 NULL );
	}
	return( EXIT_FAILURE );

}
