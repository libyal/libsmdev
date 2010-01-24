/*
 * Character type string functions
 *
 * Copyright (c) 2006-2010, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBSMDEV_STRING_H )
#define _LIBSMDEV_STRING_H

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include <liberror.h>

#include "libsmdev_libuna.h"

#if defined( _cplusplus )
extern "C" {
#endif

/* The internal string type contains UTF-8
 */
typedef libuna_utf8_character_t libsmdev_character_t;

#define PRIc_LIBSMDEV	"c"
#define PRIs_LIBSMDEV	"s"

#define _LIBSMDEV_STRING( string ) \
	(libsmdev_character_t *) string

#define libsmdev_string_compare( string1, string2, size ) \
	narrow_string_compare( (char *) string1, (char *) string2, size )

#define libsmdev_string_copy( destination, source, size ) \
	narrow_string_copy( (char *) destination, (char *) source, size )

#define libsmdev_string_length( string ) \
	narrow_string_length( (char *) string )

#define libsmdev_string_search( string, character, size ) \
	(libsmdev_character_t *) narrow_string_search( (char *) string, (int) character, size )

#define libsmdev_string_search_reverse( string, character, size ) \
	(libsmdev_character_t *) narrow_string_search_reverse( (char *) string, (int) character, size )

#if defined( __BORLANDC__ ) && ( __BORLANDC__ < 0x0560 )
#define libsmdev_string_snprintf \
	narrow_string_snprintf

#else
#define libsmdev_string_snprintf( target, size, format, ... ) \
	narrow_string_snprintf( (char *) target, size, format, __VA_ARGS__ )
#endif

#define libsmdev_string_to_int64( string, end_of_string, base ) \
	narrow_string_to_signed_long_long( (char *) string, (char **) end_of_string, base )

#define libsmdev_string_to_uint64( string, end_of_string, base ) \
	narrow_string_to_unsigned_long_long( (char *) string, (char **) end_of_string, base )

ssize_t libsmdev_string_trim_copy_from_byte_stream(
         libsmdev_character_t *string,
         size_t string_size,
         const uint8_t *byte_stream,
         size_t byte_stream_size,
         liberror_error_t **error );

#if defined( _cplusplus )
}
#endif

#endif

