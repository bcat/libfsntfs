/*
 * Info handle
 *
 * Copyright (C) 2010-2015, Joachim Metz <joachim.metz@gmail.com>
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

#include "fsntfstools_libbfio.h"
#include "fsntfstools_libcerror.h"
#include "fsntfstools_libclocale.h"
#include "fsntfstools_libcstring.h"
#include "fsntfstools_libcsystem.h"
#include "fsntfstools_libfdatetime.h"
#include "fsntfstools_libfguid.h"
#include "fsntfstools_libfsntfs.h"
#include "fsntfstools_libfusn.h"
#include "info_handle.h"

#if !defined( LIBFSNTFS_HAVE_BFIO )
extern \
int libfsntfs_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

extern \
int libfsntfs_volume_open_file_io_handle(
     libfsntfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsntfs_error_t **error );

extern \
int libfsntfs_mft_metadata_file_open_file_io_handle(
     libfsntfs_mft_metadata_file_t *mft_metadata_file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsntfs_error_t **error );
#endif

#define INFO_HANDLE_NOTIFY_STREAM	stdout

/* Prints the file attribute flags to the notify stream
 */
void info_handle_file_attribute_flags_fprint(
      uint32_t file_attribute_flags,
      FILE *notify_stream )
{
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_READ_ONLY ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs read-only (FILE_ATTRIBUTE_READ_ONLY)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_HIDDEN ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs hidden (FILE_ATTRIBUTE_HIDDEN)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SYSTEM ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs system (FILE_ATTRIBUTE_SYSTEM)\n" );
	}

	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs directory (FILE_ATTRIBUTE_DIRECTORY)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ARCHIVE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tShould be archived (FILE_ATTRIBUTE_ARCHIVE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs device (FILE_ATTRIBUTE_DEVICE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NORMAL ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs normal (FILE_ATTRIBUTE_NORMAL)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_TEMPORARY ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs temporary (FILE_ATTRIBUTE_TEMPORARY)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_SPARSE_FILE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs a sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_REPARSE_POINT ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs a reparse point or symbolic link (FILE_ATTRIBUTE_FLAG_REPARSE_POINT)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_COMPRESSED ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs compressed (FILE_ATTRIBUTE_COMPRESSED)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_OFFLINE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs offline (FILE_ATTRIBUTE_OFFLINE)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_NOT_CONTENT_INDEXED ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tContent should not be indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n" );
	}
	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_ENCRYPTED ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n" );
	}

	if( ( file_attribute_flags & LIBFSNTFS_FILE_ATTRIBUTE_FLAG_VIRTUAL ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tIs virtual (FILE_ATTRIBUTE_VIRTUAL)\n" );
	}
}

/* Retrieves the attribute type description
 */
const char *info_handle_get_attribute_type_description(
             uint32_t attribute_type )
{
	switch( attribute_type )
	{
		case LIBFSNTFS_ATTRIBUTE_TYPE_UNUSED:
			return( "Unused" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			return( "$STANDARD_INFORMATION" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST:
			return( "$ATTRIBUTE_LIST" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
			return( "$FILE_NAME" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			return( "$OBJECT_ID" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
			return( "$SECURITY_DESCRIPTOR" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
			return( "$VOLUME_NAME" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			return( "$VOLUME_INFORMATION" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_DATA:
			return( "$DATA" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			return( "$INDEX_ROOT" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			return( "$INDEX_ALLOCATION" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
			return( "$BITMAP" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			return( "$REPARSE_POINT" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_EXTENDED_INFORMATION:
			return( "$EA_INFORMATION" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_EXTENDED:
			return( "$EA" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_PROPERTY_SET:
			return( "$PROPERTY_SET" );

		case LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
			return( "$LOGGED_UTILITY_STREAM" );
	}
	return( "Unknown" );
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
	if( libbfio_file_range_initialize(
	     &( ( *info_handle )->input_file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file IO handle.",
		 function );

		goto on_error;
	}
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_file_io_handle != NULL )
		{
			libbfio_handle_free(
			 &( ( *info_handle )->input_file_io_handle ),
			 NULL );
		}
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
		if( ( *info_handle )->input_volume != NULL )
		{
			if( libfsntfs_volume_free(
			     &( ( *info_handle )->input_volume ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input volume.",
				 function );

				result = -1;
			}
		}
		if( ( *info_handle )->input_mft_metadata_file != NULL )
		{
			if( libfsntfs_mft_metadata_file_free(
			     &( ( *info_handle )->input_mft_metadata_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input MFT metadata file.",
				 function );

				result = -1;
			}
		}
		if( libbfio_handle_free(
		     &( ( *info_handle )->input_file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file IO handle.",
			 function );

			result = -1;
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
	info_handle->abort = 1;

	if( info_handle->input_volume != NULL )
	{
		if( libfsntfs_volume_signal_abort(
		     info_handle->input_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input volume to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the volume offset
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_volume_offset";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

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
	string_length = libcstring_system_string_length(
	                 string );

	if( libcsystem_string_decimal_copy_to_64_bit(
	     string,
	     string_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to 64-bit decimal.",
		 function );

		return( -1 );
	}
	info_handle->volume_offset = (off64_t) value_64bit;

	return( 1 );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function  = "info_handle_open_input";
	size_t filename_length = 0;
	int result             = 0;

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
	filename_length = libcstring_system_string_length(
	                   filename );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     info_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file name.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_set(
	     info_handle->input_file_io_handle,
	     info_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set range.",
		 function );

		goto on_error;
	}
	result = libfsntfs_check_volume_signature_file_io_handle(
	          info_handle->input_file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to check volume signature.",
		 function );

		goto on_error;
	}
	if( result != 0 )
	{
		if( libfsntfs_volume_initialize(
		     &( info_handle->input_volume ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input volume.",
			 function );

			goto on_error;
		}
		if( libfsntfs_volume_open_file_io_handle(
		     info_handle->input_volume,
		     info_handle->input_file_io_handle,
		     LIBFSNTFS_OPEN_READ,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open input volume.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libbfio_file_range_set(
		     info_handle->input_file_io_handle,
		     0,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set range.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_metadata_file_initialize(
		     &( info_handle->input_mft_metadata_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize input MFT metadata file.",
			 function );

			goto on_error;
		}
		if( libfsntfs_mft_metadata_file_open_file_io_handle(
		     info_handle->input_mft_metadata_file,
		     info_handle->input_file_io_handle,
		     LIBFSNTFS_OPEN_READ,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open input MFT metadata file.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( info_handle->input_mft_metadata_file != NULL )
	{
		libfsntfs_mft_metadata_file_free(
		 &( info_handle->input_mft_metadata_file ),
		 NULL );
	}
	if( info_handle->input_volume != NULL )
	{
		libfsntfs_volume_free(
		 &( info_handle->input_volume ),
		 NULL );
	}
	return( -1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close_input";

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
	if( info_handle->input_volume != NULL )
	{
		if( libfsntfs_volume_close(
		     info_handle->input_volume,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input volume.",
			 function );

			return( -1 );
		}
	}
	if( info_handle->input_mft_metadata_file != NULL )
	{
		if( libfsntfs_mft_metadata_file_close(
		     info_handle->input_mft_metadata_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close input MFT metadata file.",
			 function );

			return( -1 );
		}
	}
	return( 0 );
}

/* Prints attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     int attribute_index,
     libfsntfs_error_t **error )
{
	libcstring_system_character_t *value_string = NULL;
	static char *function                       = "info_handle_attribute_fprint";
	size_t value_string_size                    = 0;
	uint32_t attribute_type                     = 0;
	int result                                  = 0;

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
	fprintf(
	 info_handle->notify_stream,
	 "Attribute: %d\n",
	 attribute_index + 1 );

	if( libfsntfs_attribute_get_type(
	     attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute type.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tType\t\t\t\t: %s (0x%08" PRIx32 ")\n",
	 info_handle_get_attribute_type_description(
	  attribute_type ),
	 attribute_type );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_attribute_get_utf16_name_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libfsntfs_attribute_get_utf8_name_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = libcstring_system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_attribute_get_utf16_name(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfsntfs_attribute_get_utf8_name(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tName\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	switch( attribute_type )
	{
		case LIBFSNTFS_ATTRIBUTE_TYPE_ATTRIBUTE_LIST:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_DATA:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
			if( info_handle_file_name_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print file name attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			if( info_handle_object_identifier_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print object identifier attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			if( info_handle_standard_information_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print standard information attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			if( info_handle_volume_information_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print volume information attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
			if( info_handle_volume_name_attribute_fprint(
			     info_handle,
			     attribute,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print volume name attribute: %d information.",
				 function,
				 attribute_index );

				goto on_error;
			}
			break;

		default:
			break;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints $STANDARD_INFORMATION attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error )
{
	libcstring_system_character_t filetime_string[ 32 ];

	libcstring_system_character_t *value_string = NULL;
	libfdatetime_filetime_t *filetime           = NULL;
	static char *function                       = "info_handle_file_name_attribute_fprint";
	size_t value_string_size                    = 0;
	uint64_t value_64bit                        = 0;
	uint32_t value_32bit                        = 0;
	int result                                  = 0;

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
	if( libfsntfs_file_name_attribute_get_parent_file_reference(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent file reference.",
		 function );

		goto on_error;
	}
	if( value_64bit == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tParent file reference\t\t\t: %" PRIu64 "\n",
		 value_64bit );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tParent file reference\t\t: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
		 value_64bit & 0xffffffffffffUL,
		 value_64bit >> 48 );
	}
	if( libfdatetime_filetime_initialize(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create filetime.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_attribute_get_creation_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tCreation time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 filetime_string );

	if( libfsntfs_file_name_attribute_get_modification_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tModification time\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 filetime_string );

	if( libfsntfs_file_name_attribute_get_access_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tAccess time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 filetime_string );

	if( libfsntfs_file_name_attribute_get_entry_modification_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modifiation time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tEntry modification time\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 filetime_string );

	if( libfdatetime_filetime_free(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free filetime.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_name_attribute_get_file_attribute_flags(
	     attribute,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile attribute flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_file_attribute_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_file_name_attribute_get_utf16_name_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libfsntfs_file_name_attribute_get_utf8_name_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = libcstring_system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_file_name_attribute_get_utf16_name(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfsntfs_file_name_attribute_get_utf8_name(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tName\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
	return( -1 );
}

/* Prints $OBJECT_ID attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_object_identifier_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error )
{
	uint8_t guid_data[ 16 ];

	libcstring_system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid = NULL;
	static char *function       = "info_handle_object_identifier_attribute_fprint";
	int result                  = 0;

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
	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
	if( libfsntfs_object_identifier_attribute_get_droid_file_identifier(
	     attribute,
	     guid_data,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve droid file identifier.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     guid_data,
	     16,
	     LIBFGUID_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to GUID.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfguid_identifier_copy_to_utf16_string(
		  guid,
		  (uint16_t *) guid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#else
	result = libfguid_identifier_copy_to_utf8_string(
		  guid,
		  (uint8_t *) guid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tDroid file identifier\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
	 guid_string );

	result = libfsntfs_object_identifier_attribute_get_birth_droid_volume_identifier(
	          attribute,
	          guid_data,
	          16,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve birth droid volume identifier.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     guid_data,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tBirth droid volume identifier\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 guid_string );
	}
	result = libfsntfs_object_identifier_attribute_get_birth_droid_file_identifier(
	          attribute,
	          guid_data,
	          16,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve birth droid file identifier.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     guid_data,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tBirth droid file identifier\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 guid_string );
	}
	result = libfsntfs_object_identifier_attribute_get_birth_droid_domain_identifier(
	          attribute,
	          guid_data,
	          16,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve birth droid domain identifier.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     guid_data,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tBirth droid domain identifier\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 guid_string );
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints $STANDARD_INFORMATION attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_standard_information_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error )
{
	libcstring_system_character_t filetime_string[ 32 ];

	libfdatetime_filetime_t *filetime = NULL;
	static char *function             = "info_handle_standard_information_attribute_fprint";
	uint64_t value_64bit              = 0;
	uint32_t value_32bit              = 0;
	int result                        = 0;

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
	if( libfdatetime_filetime_initialize(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create filetime.",
		 function );

		goto on_error;
	}
	if( libfsntfs_standard_information_attribute_get_creation_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tCreation time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 filetime_string );

	if( libfsntfs_standard_information_attribute_get_modification_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tModification time\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 filetime_string );

	if( libfsntfs_standard_information_attribute_get_access_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tAccess time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 filetime_string );

	if( libfsntfs_standard_information_attribute_get_entry_modification_time(
	     attribute,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry modifiation time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tEntry modification time\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 filetime_string );

	if( libfdatetime_filetime_free(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free filetime.",
		 function );

		goto on_error;
	}
	result = libfsntfs_standard_information_attribute_get_owner_identifier(
	          attribute,
	          &value_32bit,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve owner identifier.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tOwner identifier\t\t: %" PRIu32 "\n",
		 value_32bit );
	}
	result = libfsntfs_standard_information_attribute_get_security_identifier(
	          attribute,
	          &value_32bit,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security identifier.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tSecurity identifier\t\t: %" PRIu32 "\n",
		 value_32bit );
	}
	result = libfsntfs_standard_information_attribute_get_update_sequence_number(
	          attribute,
	          &value_64bit,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update sequence number.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tUpdate sequence number\t\t: %" PRIu64 "\n",
		 value_64bit );
	}
	if( libfsntfs_standard_information_attribute_get_file_attribute_flags(
	     attribute,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile attribute flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_file_attribute_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );

	return( 1 );

on_error:
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
	return( -1 );
}

/* Prints $VOLUME_INFORMATION attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_information_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error )
{
	static char *function = "info_handle_volume_information_attribute_fprint";
	uint16_t value_16bit  = 0;
	uint8_t major_version = 0;
	uint8_t minor_version = 0;

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
	if( libfsntfs_volume_information_attribute_get_version(
	     attribute,
	     &major_version,
	     &minor_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve version.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tVersion\t\t\t: %" PRIu8 ".%" PRIu8 "\n",
	 major_version,
	 minor_version );

	if( libfsntfs_volume_information_attribute_get_flags(
	     attribute,
	     &value_16bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve flags.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFlags\t\t\t: 0x%04" PRIx16 "\n",
	 value_16bit );

	return( 1 );
}

/* Prints $VOLUME_NAME attribute information
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_name_attribute_fprint(
     info_handle_t *info_handle,
     libfsntfs_attribute_t *attribute,
     libfsntfs_error_t **error )
{
	libcstring_system_character_t *value_string = NULL;
	static char *function                       = "info_handle_volume_name_attribute_fprint";
	size_t value_string_size                    = 0;
	int result                                  = 0;

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
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_volume_name_attribute_get_utf16_name_size(
	          attribute,
	          &value_string_size,
	          error );
#else
	result = libfsntfs_volume_name_attribute_get_utf8_name_size(
	          attribute,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = libcstring_system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_volume_name_attribute_get_utf16_name(
		          attribute,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfsntfs_volume_name_attribute_get_utf8_name(
		          attribute,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tName\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints file entry information
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_entry_fprint(
     info_handle_t *info_handle,
     libfsntfs_file_entry_t *file_entry,
     int indentation_level,
     libfsntfs_error_t **error )
{
	libcstring_system_character_t *data_stream_name = NULL;
	libcstring_system_character_t *file_entry_name  = NULL;
	libfsntfs_data_stream_t *alternate_data_stream  = NULL;
	libfsntfs_file_entry_t *sub_file_entry          = NULL;
	static char *function                           = "info_handle_file_entry_fprint";
	size_t data_stream_name_size                    = 0;
	size_t file_entry_name_size                     = 0;
	uint32_t file_attribute_flags                   = 0;
	int alternate_data_stream_index                 = 0;
	int has_default_data_stream                     = 0;
	int indentation_level_iterator                  = 0;
	int number_of_alternate_data_streams            = 0;
	int number_of_sub_file_entries                  = 0;
	int result                                      = 0;
	int sub_file_entry_index                        = 0;

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
	if( libfsntfs_file_entry_get_file_attribute_flags(
	     file_entry,
	     &file_attribute_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		goto on_error;
	}
	result = libfsntfs_file_entry_has_default_data_stream(
		  file_entry,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file entry has default data stream.",
		 function );

		goto on_error;
	}
	has_default_data_stream = result;

	if( libfsntfs_file_entry_get_number_of_alternate_data_streams(
	     file_entry,
	     &number_of_alternate_data_streams,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of alternate data streams.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_get_number_of_sub_file_entries(
	     file_entry,
	     &number_of_sub_file_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_file_entry_get_utf16_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#else
	result = libfsntfs_file_entry_get_utf8_name_size(
	          file_entry,
	          &file_entry_name_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry name size.",
		 function );

		goto on_error;
	}
	if( file_entry_name_size > 0 )
	{
		file_entry_name = libcstring_system_string_allocate(
		                   file_entry_name_size );

		if( file_entry_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file entry name string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_file_entry_get_utf16_name(
		          file_entry,
		          (uint16_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#else
		result = libfsntfs_file_entry_get_utf8_name(
		          file_entry,
		          (uint8_t *) file_entry_name,
		          file_entry_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file entry name.",
			 function );

			goto on_error;
		}
		/* The file attribute flags are used to determine if the file entry
		 * is a directory in since checking for number_of_sub_file_entries != 0
		 * will skip empty directories.
		 */
		if( ( has_default_data_stream != 0 )
		 || ( ( file_attribute_flags & 0x10000000UL ) != 0 ) )
		{
			for( indentation_level_iterator = 0;
			     indentation_level_iterator < indentation_level;
			     indentation_level_iterator++ )
			{
				fprintf(
				 info_handle->notify_stream,
				 " " );
			}
			fprintf(
			 info_handle->notify_stream,
			 "%" PRIs_LIBCSTRING_SYSTEM "\n",
			 file_entry_name );
		}
		for( alternate_data_stream_index = 0;
		     alternate_data_stream_index < number_of_alternate_data_streams;
		     alternate_data_stream_index++ )
		{
			if( libfsntfs_file_entry_get_alternate_data_stream_by_index(
			     file_entry,
			     alternate_data_stream_index,
			     &alternate_data_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve alternate data stream: %d.",
				 function,
				 alternate_data_stream_index );

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_data_stream_get_utf16_name_size(
			          alternate_data_stream,
			          &data_stream_name_size,
			          error );
#else
			result = libfsntfs_data_stream_get_utf8_name_size(
			          alternate_data_stream,
			          &data_stream_name_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve alternate data stream: %d name size.",
				 function,
				 alternate_data_stream_index );

				goto on_error;
			}
			if( data_stream_name_size > 0 )
			{
				data_stream_name = libcstring_system_string_allocate(
				                    data_stream_name_size );

				if( data_stream_name == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create alternate data stream: %d name string.",
					 function,
					 alternate_data_stream_index );

					goto on_error;
				}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
				result = libfsntfs_data_stream_get_utf16_name(
					  alternate_data_stream,
					  (uint16_t *) data_stream_name,
					  data_stream_name_size,
					  error );
#else
				result = libfsntfs_data_stream_get_utf8_name(
					  alternate_data_stream,
					  (uint8_t *) data_stream_name,
					  data_stream_name_size,
					  error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve alternate data stream: %d name.",
					 function,
					 alternate_data_stream_index );

					goto on_error;
				}
				for( indentation_level_iterator = 0;
				     indentation_level_iterator < indentation_level;
				     indentation_level_iterator++ )
				{
					fprintf(
					 info_handle->notify_stream,
					 " " );
				}
				fprintf(
				 info_handle->notify_stream,
				 "%" PRIs_LIBCSTRING_SYSTEM ":%" PRIs_LIBCSTRING_SYSTEM "\n",
				 file_entry_name,
				 data_stream_name );

				memory_free(
				 data_stream_name );

				data_stream_name = NULL;
			}
			if( libfsntfs_data_stream_free(
			     &alternate_data_stream,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free alternate data stream: %d.",
				 function,
				 alternate_data_stream );

				goto on_error;
			}
		}
		memory_free(
		 file_entry_name );

		file_entry_name = NULL;
	}
	for( sub_file_entry_index = 0;
	     sub_file_entry_index < number_of_sub_file_entries;
	     sub_file_entry_index++ )
	{
		if( libfsntfs_file_entry_get_sub_file_entry_by_index(
		     file_entry,
		     sub_file_entry_index,
		     &sub_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
		if( info_handle_file_entry_fprint(
		     info_handle,
		     sub_file_entry,
		     indentation_level + 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print file entry: %d information.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
		if( libfsntfs_file_entry_free(
		     &sub_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( sub_file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	if( data_stream_name != NULL )
	{
		memory_free(
		 data_stream_name );
	}
	if( alternate_data_stream != NULL )
	{
		libfsntfs_data_stream_free(
		 &alternate_data_stream,
		 NULL );
	}
	if( file_entry_name != NULL )
	{
		memory_free(
		 file_entry_name );
	}
	return( -1 );
}

/* Prints the MFT entry information
 * Returns 1 if successful or -1 on error
 */
int info_handle_mft_entry_fprint(
     info_handle_t *info_handle,
     uint64_t mft_entry_index,
     libfsntfs_error_t **error )
{
	libfsntfs_attribute_t *attribute   = NULL;
	libfsntfs_file_entry_t *file_entry = NULL;
	static char *function              = "info_handle_mft_entry_fprint";
	int attribute_index                = 0;
	int is_allocated                   = 0;
	int number_of_attributes           = 0;
	int result                         = 0;

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
	fprintf(
	 info_handle->notify_stream,
	 "Windows NT File System information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "MFT entry: %" PRIu64 " information:\n",
	 mft_entry_index );

	if( info_handle->input_mft_metadata_file != NULL )
	{
		result = libfsntfs_mft_metadata_file_get_file_entry_by_index(
		          info_handle->input_mft_metadata_file,
		          mft_entry_index,
		          &file_entry,
		          error );
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_file_entry_by_index(
		          info_handle->input_volume,
		          mft_entry_index,
		          &file_entry,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry: %" PRIu64 ".",
		 function,
		 mft_entry_index );

		goto on_error;
	}
	is_allocated = libfsntfs_file_entry_is_allocated(
	                file_entry,
	                error );

	if( is_allocated == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file entry is allocated.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tIs allocated\t\t\t: " );

	if( is_allocated == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "false" );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "true" );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfsntfs_file_entry_get_number_of_attributes(
	     file_entry,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tNumber of attributes\t\t: %d\n",
	 number_of_attributes );

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	for( attribute_index = 0;
	     attribute_index < number_of_attributes;
	     attribute_index++ )
	{
		if( libfsntfs_file_entry_get_attribute_by_index(
		     file_entry,
		     attribute_index,
		     &attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
		if( info_handle_attribute_fprint(
		     info_handle,
		     attribute,
		     attribute_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print attribute: %d information.",
			 function,
			 attribute_index );

			goto on_error;
		}
		if( libfsntfs_attribute_free(
		     &attribute,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attribute: %d.",
			 function,
			 attribute_index );

			goto on_error;
		}
	}
	if( libfsntfs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( attribute != NULL )
	{
		libfsntfs_file_entry_free(
		 &attribute,
		 NULL );
	}
	if( file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints the MFT entries information
 * Returns 1 if successful or -1 on error
 */
int info_handle_mft_entries_fprint(
     info_handle_t *info_handle,
     libfsntfs_error_t **error )
{
	static char *function           = "info_handle_mft_entries_fprint";
	uint64_t file_entry_index       = 0;
	uint64_t number_of_file_entries = 0;
	int result                      = 0;

	if( info_handle->input_mft_metadata_file != NULL )
	{
		result = libfsntfs_mft_metadata_file_get_number_of_file_entries(
		          info_handle->input_mft_metadata_file,
		          &number_of_file_entries,
		          error );
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_number_of_file_entries(
		          info_handle->input_volume,
		          &number_of_file_entries,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of file entries.",
		 function );

		return( -1 );
	}
	if( number_of_file_entries > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid nubmer of file entries value out of bounds.",
		 function );

		return( -1 );
	}
	for( file_entry_index = 0;
	     file_entry_index < number_of_file_entries;
	     file_entry_index++ )
	{
		if( info_handle_mft_entry_fprint(
		     info_handle,
		     file_entry_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print MFT entry: %" PRIu64 ".",
			 function,
			 file_entry_index );

			return( -1 );
		}
	}
	return( 1 );
}

/* Prints the system hierarchy entry information
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_system_hierarchy_fprint(
     info_handle_t *info_handle,
     libfsntfs_error_t **error )
{
	libfsntfs_file_entry_t *file_entry = NULL;
	static char *function              = "info_handle_file_system_hierarchy_fprint";

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
	fprintf(
	 info_handle->notify_stream,
	 "Windows NT File System information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "File system hierarchy:\n" );

	if( libfsntfs_volume_get_root_directory(
	     info_handle->input_volume,
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root directory file entry.",
		 function );

		goto on_error;
	}
	if( info_handle_file_entry_fprint(
	     info_handle,
	     file_entry,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print root directory file entry information.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints the user journal ($UsnJrnl) record update reason to the notify stream
 */
void info_handle_usn_record_update_reason_flags_fprint(
      uint32_t update_reason_flags,
      FILE *notify_stream )
{
/* TODO add description */
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_DATA_OVERWRITE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_DATA_OVERWRITE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_DATA_EXTEND ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_DATA_EXTEND)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_DATA_TRUNCATION ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_DATA_TRUNCATION)\n" );
	}

	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_NAMED_DATA_OVERWRITE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_NAMED_DATA_OVERWRITE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_NAMED_DATA_EXTEND ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_NAMED_DATA_EXTEND)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_NAMED_DATA_TRUNCATION ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_NAMED_DATA_TRUNCATION)\n" );
	}

	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_FILE_CREATE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_FILE_CREATE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_FILE_DELETE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_FILE_DELETE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_EXTENDED_ATTRIBUTE_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_EA_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_SECURITY_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_SECURITY_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_RENAME_OLD_NAME ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_RENAME_OLD_NAME)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_RENAME_NEW_NAME ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_RENAME_NEW_NAME)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_INDEXABLE_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_INDEXABLE_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_BASIC_INFO_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_BASIC_INFO_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_HARD_LINK_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_HARD_LINK_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_COMPRESSION_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_COMPRESSION_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_ENCRYPTION_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_ENCRYPTION_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_OBJECT_IDENTIFIER_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_OBJECT_IDENTIFIER_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_REPARSE_POINT_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_REPARSE_POINT_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_STREAM_CHANGE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_STREAM_CHANGE)\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_UKNOWN_0x00400000 ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tUnknown 0x00400000\n" );
	}
	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_CLOSE ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_REASON_CLOSE)\n" );
	}

	if( ( update_reason_flags & LIBFUSN_UPDATE_REASON_FLAG_UKNOWN_0x80000000 ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\tUnknown 0x80000000\n" );
	}
}

/* Prints the user journal ($UsnJrnl) record update source flags to the notify stream
 */
void info_handle_usn_record_update_source_flags_fprint(
      uint32_t update_source_flags,
      FILE *notify_stream )
{
/* TODO add description */
	if( ( update_source_flags & LIBFUSN_UPDATE_SOURCE_FLAG_DATA_MANAGEMENT ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_SOURCE_DATA_MANAGEMENT)\n" );
	}
	if( ( update_source_flags & LIBFUSN_UPDATE_SOURCE_FLAG_AUXILIARY_DATA ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_SOURCE_AUXILIARY_DATA)\n" );
	}
	if( ( update_source_flags & LIBFUSN_UPDATE_SOURCE_FLAG_REPLICATION_MANAGEMENT ) != 0 )
	{
		fprintf(
		 notify_stream,
		 "\t\t(USN_SOURCE_REPLICATION_MANAGEMENT)\n" );
	}
}

/* Prints the user journal ($UsnJrnl) record information
 * Returns 1 if successful or -1 on error
 */
int info_handle_usn_record_fprint(
     info_handle_t *info_handle,
     libfusn_record_t *usn_record,
     libfsntfs_error_t **error )
{
	libcstring_system_character_t filetime_string[ 32 ];

	libcstring_system_character_t *value_string = NULL;
	libfdatetime_filetime_t *filetime           = NULL;
	static char *function                       = "info_handle_usn_record_fprint";
	size_t value_string_size                    = 0;
	uint64_t value_64bit                        = 0;
	uint32_t value_32bit                        = 0;
	int result                                  = 0;

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
	fprintf(
	 info_handle->notify_stream,
	 "USN record:\n" );

	if( libfdatetime_filetime_initialize(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create filetime.",
		 function );

		goto on_error;
	}
	if( libfusn_record_get_update_time(
	     usn_record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_64bit(
	     filetime,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime from 64-bit.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfdatetime_filetime_copy_to_utf16_string(
		  filetime,
		  (uint16_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#else
	result = libfdatetime_filetime_copy_to_utf8_string(
		  filetime,
		  (uint8_t *) filetime_string,
		  32,
		  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tUpdate time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
	 filetime_string );

	if( libfdatetime_filetime_free(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free filetime.",
		 function );

		goto on_error;
	}
	if( libfusn_record_get_update_sequence_number(
	     usn_record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update sequence number.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tUpdate sequence number\t\t: %" PRIu64 "\n",
	 value_64bit );

	if( libfusn_record_get_update_reason_flags(
	     usn_record,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update reason flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tUpdate reason flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_usn_record_update_reason_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfusn_record_get_update_source_flags(
	     usn_record,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve update source flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tUpdate source flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_usn_record_update_source_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t: " );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfusn_record_get_utf16_name_size(
	          usn_record,
	          &value_string_size,
	          error );
#else
	result = libfusn_record_get_utf8_name_size(
	          usn_record,
	          &value_string_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name size.",
		 function );

		goto on_error;
	}
	if( value_string_size > 0 )
	{
		value_string = libcstring_system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfusn_record_get_utf16_name(
		          usn_record,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfusn_record_get_utf8_name(
		          usn_record,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfusn_record_get_file_reference(
	     usn_record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file reference.",
		 function );

		goto on_error;
	}
	if( value_64bit == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tFile reference\t\t\t: %" PRIu64 "\n",
		 value_64bit );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tFile reference\t\t\t: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
		 value_64bit & 0xffffffffffffUL,
		 value_64bit >> 48 );
	}
	if( libfusn_record_get_parent_file_reference(
	     usn_record,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve parent file reference.",
		 function );

		goto on_error;
	}
	if( value_64bit == 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tParent file reference\t\t\t: %" PRIu64 "\n",
		 value_64bit );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tParent file reference\t\t: MFT entry: %" PRIu64 ", sequence: %" PRIu64 "\n",
		 value_64bit & 0xffffffffffffUL,
		 value_64bit >> 48 );
	}
	if( libfusn_record_get_file_attribute_flags(
	     usn_record,
	     &value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFile attribute flags\t\t: 0x%08" PRIx32 "\n",
	 value_32bit );
	info_handle_file_attribute_flags_fprint(
	 value_32bit,
	 info_handle->notify_stream );
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints the user journal ($UsnJrnl) entry information
 * Returns 1 if successful or -1 on error
 */
int info_handle_user_journal_fprint(
     info_handle_t *info_handle,
     libfsntfs_error_t **error )
{
	libcstring_system_character_t *name            = NULL;
	libcstring_system_character_t *path            = NULL;
	libfsntfs_data_stream_t *alternate_data_stream = NULL;
	libfsntfs_file_entry_t *file_entry             = NULL;
	libfusn_record_t *usn_record                   = NULL;
	uint8_t *buffer                                = NULL;
	static char *function                          = "info_handle_user_journal_fprint";
	off64_t cluster_block_offset                   = 0;
	off64_t current_offset                         = 0;
	off64_t extent_offset                          = 0;
	size64_t data_size                             = 0;
	size64_t extent_size                           = 0;
	size_t buffer_offset                           = 0;
	size_t cluster_block_size                      = 0;
	size_t name_length                             = 0;
	size_t path_length                             = 0;
	ssize_t read_count                             = 0;
	uint32_t extent_flags                          = 0;
	uint32_t usn_record_size                       = 0;
	int extent_index                               = 0;
	int number_of_extents                          = 0;
	int result                                     = 0;

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
	fprintf(
	 info_handle->notify_stream,
	 "Windows NT File System information:\n\n" );

	path = _LIBCSTRING_SYSTEM_STRING( "\\$Extend\\$UsnJrnl" );

	path_length = libcstring_system_string_length(
	               path );

	fprintf(
	 info_handle->notify_stream,
	 "User journal: %" PRIs_LIBCSTRING_SYSTEM "\n\n",
	 path );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_volume_get_file_entry_by_utf16_path(
	          info_handle->input_volume,
	          (uint16_t *) path,
	          path_length,
	          &file_entry,
	          error );
#else
	result = libfsntfs_volume_get_file_entry_by_utf8_path(
	          info_handle->input_volume,
	          (uint8_t *) path,
	          path_length,
	          &file_entry,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file entry: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 path );

		goto on_error;
	}
	name = _LIBCSTRING_SYSTEM_STRING( "$J" );

	name_length = libcstring_system_string_length(
	               name );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsntfs_file_entry_get_alternate_data_stream_by_utf16_name(
	          file_entry,
	          (uint16_t *) name,
	          name_length,
	          &alternate_data_stream,
	          error );
#else
	result = libfsntfs_file_entry_get_alternate_data_stream_by_utf8_name(
	          file_entry,
	          (uint8_t *) name,
	          name_length,
	          &alternate_data_stream,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve alternate data stream: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 name );

		goto on_error;
	}
	if( libfsntfs_volume_get_cluster_block_size(
	     info_handle->input_volume,
	     &cluster_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cluster block size.",
		 function );

		goto on_error;
	}
	if( libfsntfs_data_stream_get_size(
	     alternate_data_stream,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve alternate data stream size.",
		 function );

		goto on_error;
	}
	buffer = (uint8_t *) memory_allocate(
	                      sizeof( uint8_t ) * cluster_block_size );

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create buffer.",
		 function );

		goto on_error;
	}
	if( libfsntfs_data_stream_get_number_of_extents(
	     alternate_data_stream,
	     &number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve alternate data stream number of extents.",
		 function );

		goto on_error;
	}
/* TODO check if extent size is within data size */
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libfsntfs_data_stream_get_extent(
		     alternate_data_stream,
		     extent_index,
		     &extent_offset,
		     &extent_size,
		     &extent_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve alternate data stream extent: %d.",
			 function,
			 extent_index );

			goto on_error;
		}
		if( ( extent_flags & LIBFSNTFS_EXTENT_FLAG_IS_SPARSE ) != 0 )
		{
			continue;
		}
		if( libfsntfs_data_stream_seek_offset(
		     alternate_data_stream,
		     extent_offset,
		     SEEK_SET,
		     error ) != extent_offset )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset: 0x%08" PRIx64 " in alternate data stream: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 extent_offset,
			 name );

			goto on_error;
		}
		cluster_block_offset = 0;

/* TODO check if extent size is multitude of cluster block size */
		while( (size64_t) cluster_block_offset < extent_size )
		{
/* TODO work around for remnant data in buffer */
			if( memory_set(
			     buffer,
			     0,
			     cluster_block_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear buffer.",
				 function );

				goto on_error;
			}
			read_count = libfsntfs_data_stream_read_buffer(
				      alternate_data_stream,
				      buffer,
				      cluster_block_size,
				      error );

			if( read_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read from alternate data stream: %" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 name );

				goto on_error;
			}
			else if( result == 0 )
			{
				break;
			}
			buffer_offset = 0;

/* TODO do an empty block check
			if( buffer[ 0 ] == 0 )
			{
				continue;
			}
*/
			while( buffer_offset < ( (size_t) read_count - 60 ) )
			{
				current_offset = extent_offset + cluster_block_offset + (off64_t) buffer_offset;

				if( (size64_t) current_offset >= data_size )
				{
					break;
				}
				if( buffer[ buffer_offset ] == 0 )
				{
					break;
				}
				if( libfusn_record_initialize(
				     &usn_record,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to initialize USN record.",
					 function );

					goto on_error;
				}
				if( libfusn_record_copy_from_byte_stream(
				     usn_record,
				     &( buffer[ buffer_offset ] ),
				     read_count,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to copy USN record from byte stream at offset: 0x%08" PRIx64 ".",
					 function,
					 current_offset );

					goto on_error;
				}
				if( libfusn_record_get_size(
				     usn_record,
				     &usn_record_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve USN record size.",
					 function );

					goto on_error;
				}
				if( info_handle_usn_record_fprint(
				     info_handle,
				     usn_record,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print USN record information.",
					 function );

					goto on_error;
				}
				if( libfusn_record_free(
				     &usn_record,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free USN record.",
					 function );

					goto on_error;
				}
				buffer_offset += usn_record_size;
			}
			cluster_block_offset += read_count;
		}
	}
	memory_free(
	 buffer );

	buffer = NULL;

	if( libfsntfs_data_stream_free(
	     &alternate_data_stream,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free alternate data stream.",
		 function );

		goto on_error;
	}
	if( libfsntfs_file_entry_free(
	     &file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file entry.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( usn_record != NULL )
	{
		libfusn_record_free(
		 &usn_record,
		 NULL );
	}
	if( buffer != NULL )
	{
		memory_free(
		 buffer );
	}
	if( alternate_data_stream != NULL )
	{
		libfsntfs_data_stream_free(
		 &alternate_data_stream,
		 NULL );
	}
	if( file_entry != NULL )
	{
		libfsntfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( -1 );
}

/* Prints the volume information
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libcstring_system_character_t *volume_name = NULL;
	static char *function                      = "info_handle_volume_fprint";
	size_t cluster_block_size                  = 0;
	size_t volume_name_size                    = 0;
	int result                                 = 0;
	uint8_t major_version                      = 0;
	uint8_t minor_version                      = 0;

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
	fprintf(
	 info_handle->notify_stream,
	 "Windows NT File System information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "Volume information:\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t: " );

	if( info_handle->input_mft_metadata_file != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_mft_metadata_file_get_utf16_volume_name_size(
		          info_handle->input_mft_metadata_file,
		          &volume_name_size,
		          error );
#else
		result = libfsntfs_mft_metadata_file_get_utf8_volume_name_size(
		          info_handle->input_mft_metadata_file,
		          &volume_name_size,
		          error );
#endif
	}
	else if( info_handle->input_volume != NULL )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfsntfs_volume_get_utf16_name_size(
		          info_handle->input_volume,
		          &volume_name_size,
		          error );
#else
		result = libfsntfs_volume_get_utf8_name_size(
		          info_handle->input_volume,
		          &volume_name_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume name size.",
		 function );

		goto on_error;
	}
	if( volume_name_size > 0 )
	{
		volume_name = libcstring_system_string_allocate(
		               volume_name_size );

		if( volume_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create volume name string.",
			 function );

			goto on_error;
		}
		if( info_handle->input_mft_metadata_file != NULL )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_mft_metadata_file_get_utf16_volume_name(
			          info_handle->input_mft_metadata_file,
			          (uint16_t *) volume_name,
			          volume_name_size,
			          error );
#else
			result = libfsntfs_mft_metadata_file_get_utf8_volume_name(
			          info_handle->input_mft_metadata_file,
			          (uint8_t *) volume_name,
			          volume_name_size,
			          error );
#endif
		}
		else if( info_handle->input_volume != NULL )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfsntfs_volume_get_utf16_name(
			          info_handle->input_volume,
			          (uint16_t *) volume_name,
			          volume_name_size,
			          error );
#else
			result = libfsntfs_volume_get_utf8_name(
			          info_handle->input_volume,
			          (uint8_t *) volume_name,
			          volume_name_size,
			          error );
#endif
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume name.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_LIBCSTRING_SYSTEM "",
		 volume_name );

		memory_free(
		 volume_name );

		volume_name = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( info_handle->input_mft_metadata_file != NULL )
	{
		result = libfsntfs_mft_metadata_file_get_volume_version(
		          info_handle->input_mft_metadata_file,
		          &major_version,
		          &minor_version,
		          error );
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_version(
		          info_handle->input_volume,
		          &major_version,
		          &minor_version,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve version.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tVersion\t\t\t\t: %" PRIu8 ".%" PRIu8 "\n",
	 major_version,
	 minor_version );

	if( info_handle->input_mft_metadata_file != NULL )
	{
/* TODO */
	}
	else if( info_handle->input_volume != NULL )
	{
		result = libfsntfs_volume_get_cluster_block_size(
		          info_handle->input_volume,
		          &cluster_block_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cluster block size.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tCluster block size\t\t: %" PRIzd "\n",
	 cluster_block_size );

/* TODO print more info */

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( volume_name != NULL )
	{
		memory_free(
		 volume_name );
	}
	return( -1 );
}

