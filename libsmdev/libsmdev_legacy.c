/*
 * Legacy functions
 *
 * Copyright (c) 2010-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <types.h>

#include <liberror.h>

#include "libsmdev_legacy.h"
#include "libsmdev_metadata.h"
#include "libsmdev_types.h"

#if !defined( HAVE_LOCAL_LIBSMDEV )

/* Retrieves the amount of read/write error retries
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_get_amount_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t *amount_of_error_retries,
     liberror_error_t **error )
{
	return( libsmdev_handle_get_number_of_error_retries(
	         handle,
	         amount_of_error_retries,
	         error ) );
}

/* Sets the amount of read/write error retries
 * Returns the 1 if succesful or -1 on error
 */
int libsmdev_handle_set_amount_of_error_retries(
     libsmdev_handle_t *handle,
     uint8_t amount_of_error_retries,
     liberror_error_t **error )
{
	return( libsmdev_handle_set_number_of_error_retries(
	         handle,
	         amount_of_error_retries,
	         error ) );
}

#endif /* !defined( HAVE_LOCAL_LIBSMDEV ) */

