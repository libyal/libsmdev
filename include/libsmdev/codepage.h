/*
 * Codepage definitions for libsmdev
 *
 * Copyright (C) 2010-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBSMDEV_CODEPAGE_H )
#define _LIBSMDEV_CODEPAGE_H

#include <libsmdev/types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* The codepage definitions
 */
enum LIBSMDEV_CODEPAGES
{
	LIBSMDEV_CODEPAGE_ASCII				= 20127,

	LIBSMDEV_CODEPAGE_ISO_8859_1			= 28591,
	LIBSMDEV_CODEPAGE_ISO_8859_2			= 28592,
	LIBSMDEV_CODEPAGE_ISO_8859_3			= 28593,
	LIBSMDEV_CODEPAGE_ISO_8859_4			= 28594,
	LIBSMDEV_CODEPAGE_ISO_8859_5			= 28595,
	LIBSMDEV_CODEPAGE_ISO_8859_6			= 28596,
	LIBSMDEV_CODEPAGE_ISO_8859_7			= 28597,
	LIBSMDEV_CODEPAGE_ISO_8859_8			= 28598,
	LIBSMDEV_CODEPAGE_ISO_8859_9			= 28599,
	LIBSMDEV_CODEPAGE_ISO_8859_10			= 28600,
	LIBSMDEV_CODEPAGE_ISO_8859_11			= 28601,
	LIBSMDEV_CODEPAGE_ISO_8859_13			= 28603,
	LIBSMDEV_CODEPAGE_ISO_8859_14			= 28604,
	LIBSMDEV_CODEPAGE_ISO_8859_15			= 28605,
	LIBSMDEV_CODEPAGE_ISO_8859_16			= 28606,

	LIBSMDEV_CODEPAGE_KOI8_R			= 20866,
	LIBSMDEV_CODEPAGE_KOI8_U			= 21866,

	LIBSMDEV_CODEPAGE_WINDOWS_874			= 874,
	LIBSMDEV_CODEPAGE_WINDOWS_932			= 932,
	LIBSMDEV_CODEPAGE_WINDOWS_936			= 936,
	LIBSMDEV_CODEPAGE_WINDOWS_949			= 949,
	LIBSMDEV_CODEPAGE_WINDOWS_950			= 950,
	LIBSMDEV_CODEPAGE_WINDOWS_1250			= 1250,
	LIBSMDEV_CODEPAGE_WINDOWS_1251			= 1251,
	LIBSMDEV_CODEPAGE_WINDOWS_1252			= 1252,
	LIBSMDEV_CODEPAGE_WINDOWS_1253			= 1253,
	LIBSMDEV_CODEPAGE_WINDOWS_1254			= 1254,
	LIBSMDEV_CODEPAGE_WINDOWS_1255			= 1255,
	LIBSMDEV_CODEPAGE_WINDOWS_1256			= 1256,
	LIBSMDEV_CODEPAGE_WINDOWS_1257			= 1257,
	LIBSMDEV_CODEPAGE_WINDOWS_1258			= 1258
};

#define LIBSMDEV_CODEPAGE_US_ASCII			LIBSMDEV_CODEPAGE_ASCII

#define LIBSMDEV_CODEPAGE_ISO_WESTERN_EUROPEAN		LIBSMDEV_CODEPAGE_ISO_8859_1
#define LIBSMDEV_CODEPAGE_ISO_CENTRAL_EUROPEAN		LIBSMDEV_CODEPAGE_ISO_8859_2
#define LIBSMDEV_CODEPAGE_ISO_SOUTH_EUROPEAN		LIBSMDEV_CODEPAGE_ISO_8859_3
#define LIBSMDEV_CODEPAGE_ISO_NORTH_EUROPEAN		LIBSMDEV_CODEPAGE_ISO_8859_4
#define LIBSMDEV_CODEPAGE_ISO_CYRILLIC			LIBSMDEV_CODEPAGE_ISO_8859_5
#define LIBSMDEV_CODEPAGE_ISO_ARABIC			LIBSMDEV_CODEPAGE_ISO_8859_6
#define LIBSMDEV_CODEPAGE_ISO_GREEK			LIBSMDEV_CODEPAGE_ISO_8859_7
#define LIBSMDEV_CODEPAGE_ISO_HEBREW			LIBSMDEV_CODEPAGE_ISO_8859_8
#define LIBSMDEV_CODEPAGE_ISO_TURKISH			LIBSMDEV_CODEPAGE_ISO_8859_9
#define LIBSMDEV_CODEPAGE_ISO_NORDIC			LIBSMDEV_CODEPAGE_ISO_8859_10
#define LIBSMDEV_CODEPAGE_ISO_THAI			LIBSMDEV_CODEPAGE_ISO_8859_11
#define LIBSMDEV_CODEPAGE_ISO_BALTIC			LIBSMDEV_CODEPAGE_ISO_8859_13
#define LIBSMDEV_CODEPAGE_ISO_CELTIC			LIBSMDEV_CODEPAGE_ISO_8859_14

#define LIBSMDEV_CODEPAGE_ISO_LATIN_1			LIBSMDEV_CODEPAGE_ISO_8859_1
#define LIBSMDEV_CODEPAGE_ISO_LATIN_2			LIBSMDEV_CODEPAGE_ISO_8859_2
#define LIBSMDEV_CODEPAGE_ISO_LATIN_3			LIBSMDEV_CODEPAGE_ISO_8859_3
#define LIBSMDEV_CODEPAGE_ISO_LATIN_4			LIBSMDEV_CODEPAGE_ISO_8859_4
#define LIBSMDEV_CODEPAGE_ISO_LATIN_5			LIBSMDEV_CODEPAGE_ISO_8859_9
#define LIBSMDEV_CODEPAGE_ISO_LATIN_6			LIBSMDEV_CODEPAGE_ISO_8859_10
#define LIBSMDEV_CODEPAGE_ISO_LATIN_7			LIBSMDEV_CODEPAGE_ISO_8859_13
#define LIBSMDEV_CODEPAGE_ISO_LATIN_8			LIBSMDEV_CODEPAGE_ISO_8859_14
#define LIBSMDEV_CODEPAGE_ISO_LATIN_9			LIBSMDEV_CODEPAGE_ISO_8859_15
#define LIBSMDEV_CODEPAGE_ISO_LATIN_10			LIBSMDEV_CODEPAGE_ISO_8859_16

#define LIBSMDEV_CODEPAGE_KOI8_RUSSIAN			LIBSMDEV_CODEPAGE_KOI8_R
#define LIBSMDEV_CODEPAGE_KOI8_UKRAINIAN		LIBSMDEV_CODEPAGE_KOI8_U

#define LIBSMDEV_CODEPAGE_WINDOWS_THAI			LIBSMDEV_CODEPAGE_WINDOWS_874
#define LIBSMDEV_CODEPAGE_WINDOWS_JAPANESE		LIBSMDEV_CODEPAGE_WINDOWS_932
#define LIBSMDEV_CODEPAGE_WINDOWS_CHINESE_SIMPLIFIED	LIBSMDEV_CODEPAGE_WINDOWS_936
#define LIBSMDEV_CODEPAGE_WINDOWS_KOREAN		LIBSMDEV_CODEPAGE_WINDOWS_949
#define LIBSMDEV_CODEPAGE_WINDOWS_CHINESE_TRADITIONAL	LIBSMDEV_CODEPAGE_WINDOWS_950
#define LIBSMDEV_CODEPAGE_WINDOWS_CENTRAL_EUROPEAN	LIBSMDEV_CODEPAGE_WINDOWS_1250
#define LIBSMDEV_CODEPAGE_WINDOWS_CYRILLIC		LIBSMDEV_CODEPAGE_WINDOWS_1251
#define LIBSMDEV_CODEPAGE_WINDOWS_WESTERN_EUROPEAN	LIBSMDEV_CODEPAGE_WINDOWS_1252
#define LIBSMDEV_CODEPAGE_WINDOWS_GREEK			LIBSMDEV_CODEPAGE_WINDOWS_1253
#define LIBSMDEV_CODEPAGE_WINDOWS_TURKISH		LIBSMDEV_CODEPAGE_WINDOWS_1254
#define LIBSMDEV_CODEPAGE_WINDOWS_HEBREW		LIBSMDEV_CODEPAGE_WINDOWS_1255
#define LIBSMDEV_CODEPAGE_WINDOWS_ARABIC		LIBSMDEV_CODEPAGE_WINDOWS_1256
#define LIBSMDEV_CODEPAGE_WINDOWS_BALTIC		LIBSMDEV_CODEPAGE_WINDOWS_1257
#define LIBSMDEV_CODEPAGE_WINDOWS_VIETNAMESE		LIBSMDEV_CODEPAGE_WINDOWS_1258

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBSMDEV_CODEPAGE_H ) */

