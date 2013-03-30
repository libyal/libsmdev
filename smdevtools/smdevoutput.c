/*
 * Common output functions for the smdevtools
 *
 * Copyright (c) 2010-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "smdevoutput.h"
#include "smdevtools_libcerror.h"
#include "smdevtools_libclocale.h"
#include "smdevtools_libcnotify.h"
#include "smdevtools_libcstring.h"
#include "smdevtools_libcsystem.h"
#include "smdevtools_libsmdev.h"
#include "smdevtools_libuna.h"

/* Prints the copyright information
 */
void smdevoutput_copyright_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf(
	 stream,
	 "Copyright (c) 2010-2013, Joachim Metz <%s>.\n"
	 "This is free software; see the source for copying conditions. There is NO\n"
	 "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n",
	 PACKAGE_BUGREPORT );
}

/* Prints the version information
 */
void smdevoutput_version_fprint(
      FILE *stream,
      const char *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
        fprintf(
	 stream,
	 "%s %s\n\n",
         program,
	 LIBSMDEV_VERSION_STRING );
}

/* Prints the detailed version information
 */
void smdevoutput_version_detailed_fprint(
      FILE *stream,
      const char *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
        fprintf(
	 stream,
	 "%s %s (libsmdev %s",
         program,
	 LIBSMDEV_VERSION_STRING,
	 LIBSMDEV_VERSION_STRING );

	fprintf(
	 stream,
	 ", libuna %s",
	 LIBUNA_VERSION_STRING );

        fprintf(
	 stream,
	 ")\n\n" );
}

