AC_PREREQ( 2.59 )

AC_INIT(
 [libsmdev],
  [20100110],
  [forensics@hoffmannbv.nl])

AC_CONFIG_SRCDIR(
 [include/libsmdev.h.in])

AM_INIT_AUTOMAKE(
 [libsmdev],
 [20100110])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for configure arguments
LIBSMDEV_TEST_ENABLE(
 [wide-character-type],
 [wide_character_type],
 [enable wide character type support],
 [no])
LIBSMDEV_TEST_ENABLE(
 [verbose-output],
 [verbose_output],
 [enable verbose output],
 [no])
LIBSMDEV_TEST_ENABLE(
 [debug-output],
 [debug_output],
 [enable debug output],
 [no])

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Check for headers
dnl Headers included in include/libsmdev/types.h
AC_CHECK_HEADER(
 [sys/types.h],
 [AC_DEFINE(
  [HAVE_SYS_TYPES_H],
  [1],
  [Define to 1 if you have the <sys/types.h> header file.])
 AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [0])])

AC_CHECK_HEADER(
 [inttypes.h],
 [AC_DEFINE(
  [HAVE_INTTYPES_H],
  [1],
  [Define to 1 if you have the <inttypes.h> header file.])
 AC_SUBST(
  [HAVE_INTTYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_INTTYPES_H],
  [0])])

AC_CHECK_HEADER(
 [stdint.h],
 [AC_DEFINE(
  [HAVE_STDINT_H],
  [1],
  [Define to 1 if you have the <stdint.h> header file.])
 AC_SUBST(
  [HAVE_STDINT_H],
  [1])],
 [AC_SUBST(
  [HAVE_STDINT_H],
  [0])])

dnl Headers included in common/types.h
AC_CHECK_HEADERS([limits.h])

dnl Headers included in common/narrow_string.h
AC_CHECK_HEADERS([string.h])

dnl Headers included in common/memory.h
AC_CHECK_HEADERS([stdlib.h])

dnl Headers included in libnotify/libnotify.c
AC_CHECK_HEADERS([stdarg.h])
AC_CHECK_HEADERS([varargs.h])

dnl Headers included in libnotify/libnotify.c and libsmdev/libsmdev_handle.c
AC_CHECK_HEADERS([errno.h])

dnl Headers included in libsmdev/libsmdev_handle.c
AC_CHECK_HEADERS([sys/stat.h])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([unistd.h])

dnl Headers included in libsmdev/libsmdev_metadata.c
AC_CHECK_HEADERS([sys/disk.h])
AC_CHECK_HEADERS([sys/disklabel.h])
AC_CHECK_HEADERS([sys/ioctl.h])

AC_CHECK_HEADERS([cygwin/fs.h])

AC_CHECK_HEADERS([linux/fs.h])

dnl Headers included in libsmdev/libsmdev_ata.c
AC_CHECK_HEADERS([cygwin/hdreg.h])

AC_CHECK_HEADERS([linux/hdreg.h])

dnl Headers included in libsmdev/libsmdev_scsi.c
AC_CHECK_HEADERS([scsi/scsi.h])
AC_CHECK_HEADERS([scsi/scsi_ioctl.h])
AC_CHECK_HEADERS([scsi/sg.h])

dnl Headers included in libsmdev/libsmdev_optical_disk.c
AC_CHECK_HEADERS([linux/cdrom.h])

dnl Headers included in libsmdev/libsmdev_usb.c
AC_CHECK_HEADERS([linux/usbdevice_fs.h])
AC_CHECK_HEADERS([linux/usb/ch9.h])

dnl Other headers

dnl Check for declarations and type definitions
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

AC_CHECK_TYPE(
 [size32_t],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [ssize32_t],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [size64_t],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [ssize64_t],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [off64_t],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [1])],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [0])])

AC_CHECK_TYPE([ssize_t])
AC_CHECK_TYPE([u64])

AC_CHECK_SIZEOF([off_t])
AC_CHECK_SIZEOF([size_t])

dnl Check for functions
dnl File stream functions used in common/file_stream.h
AC_CHECK_FUNCS(
 [fclose],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fclose],
  [1]) ])

AC_CHECK_FUNCS(
 [feof],
 [],
 [AC_MSG_FAILURE(
  [Missing function: feof],
  [1]) ])

AC_CHECK_FUNCS(
 [fgets],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fgets],
  [1]) ])

AC_CHECK_FUNCS(
 [fopen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fopen],
  [1]) ])

AC_CHECK_FUNCS(
 [fread],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fread],
  [1]) ])

AC_CHECK_FUNCS(
 [fseeko],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fseeko],
  [1]) ])

AC_CHECK_FUNCS(
 [fwrite],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fwrite],
  [1]) ])

AC_CHECK_FUNCS(
 [vfprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vfprintf],
  [1]) ])

dnl Memory functions used in common/memory.h
AC_CHECK_FUNCS(
 [free],
 [],
 [AC_MSG_FAILURE(
  [Missing function: free],
  [1]) ])

AC_CHECK_FUNCS(
 [malloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: malloc],
  [1]) ])

AC_CHECK_FUNCS(
 [memcmp],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memcmp],
  [1]) ])

AC_CHECK_FUNCS(
 [memcpy],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memcpy],
  [1]) ])

AC_CHECK_FUNCS(
 [memset],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memset],
  [1]) ])

AC_CHECK_FUNCS(
 [realloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: realloc],
  [1]) ])

dnl Narrow character string functions used in common/narrow_string.h
AC_CHECK_FUNCS(
 [snprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: snprintf],
  [1]) ])

AC_CHECK_FUNCS(
 [sscanf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: sscanf],
  [1]) ])

AC_CHECK_FUNCS(
 [strlen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: strlen],
  [1]) ])

AC_CHECK_FUNCS(
 [strchr],
 [],
 [AC_CHECK_FUNCS(
  [memchr],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strchr and memchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strncmp],
 [],
 [AC_CHECK_FUNCS(
  [strcmp],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncmp and strcmp],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strncpy],
 [],
 [AC_CHECK_FUNCS(
  [strcpy],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncpy and strcpy],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strrchr],
 [],
 [AC_CHECK_FUNCS(
  [memrchr],
  [AC_CHECK_DECLS(
   [memrchr],
   [],
   [AC_MSG_FAILURE(
    [Missing declaration for function memrchr],
    [1]) ]) ],
  [AC_MSG_FAILURE(
   [Missing functions: strrchr and memrchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strtoll],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoll and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strtoull],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoull and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [vsnprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vsnprintf],
  [1]) ])

dnl Check for error string functions used in libsmdev/libsmdev_error_string.h
AC_FUNC_STRERROR_R(
 [],
 [AC_CHECK_FUNCS(
  [strerror],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strerror_r and strerror],
   [1]) ]) ])

dnl File input/output functions used in libsmdev/libsmdev_handle.h
AC_CHECK_FUNCS(
 [close],
 [],
 [AC_MSG_FAILURE(
  [Missing function: close],
  [1]) ])

AC_CHECK_FUNCS(
 [lseek],
 [],
 [AC_MSG_FAILURE(
  [Missing function: lseek],
  [1]) ])

AC_CHECK_FUNCS(
 [open],
 [],
 [AC_MSG_FAILURE(
  [Missing function: open],
  [1]) ])

AC_CHECK_FUNCS(
 [read],
 [],
 [AC_MSG_FAILURE(
  [Missing function: read],
  [1]) ])

AC_CHECK_FUNCS(
 [write],
 [],
 [AC_MSG_FAILURE(
  [Missing function: write],
  [1]) ])

dnl File input/output functions used in libsmdev/libsmdev_support.h
AC_CHECK_FUNCS(
 [fstat],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fstat],
  [1]) ])

dnl Check for printf conversion specifier support
LIBSMDEV_CHECK_PRINTF_JD
LIBSMDEV_CHECK_PRINTF_ZD

dnl Checks for libraries and library functions
dnl ac_libsmdev_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments

dnl Check for enabling libuna for Unicode string support
AC_CHECK_HEADERS(
 [libuna.h],
 [],
 [ac_libsmdev_have_libuna=no])

AS_IF(
	[test "x$ac_libsmdev_have_libuna" != xno],
	[ac_libsmdev_have_libuna=yes
	AC_CHECK_LIB(
	 una,
	 libuna_get_version,
	 [],
	 [ac_libsmdev_have_libuna=no])

	dnl Byte stream functions
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_size_from_byte_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_copy_from_byte_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_size_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_copy_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_size_from_byte_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_copy_from_byte_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_size_from_utf16,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_copy_from_utf16,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_size_from_byte_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_copy_from_byte_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_size_from_utf32,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_byte_stream_copy_from_utf32,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])

	dnl UTF-8 stream functions
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_size_from_utf8_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_copy_from_utf8_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_size_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_copy_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_size_from_utf8_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_copy_from_utf8_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_size_from_utf16,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_copy_from_utf16,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_size_from_utf8_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_copy_from_utf8_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_size_from_utf32,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_stream_copy_from_utf32,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])

	dnl UTF-16 stream functions
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_size_from_utf16_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_copy_from_utf16_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_size_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_copy_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_size_from_utf16_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_copy_from_utf16_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_size_from_utf16,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_copy_from_utf16,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_size_from_utf16_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_copy_from_utf16_stream,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_size_from_utf32,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_stream_copy_from_utf32,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])

	dnl UTF-8 string functions
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_size_from_utf16,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_copy_from_utf16,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_size_from_utf32,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf8_string_copy_from_utf32,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])

	dnl UTF-16 string functions
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_size_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf16_string_copy_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])

	dnl UTF-32 string functions
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_size_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_utf32_string_copy_from_utf8,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])

	dnl compare string functions
	AC_CHECK_LIB(
	 una,
	 libuna_compare_utf8_with_utf16,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	AC_CHECK_LIB(
	 una,
	 libuna_compare_utf8_with_utf32,
	 [ac_libsmdev_dummy=yes],
	 [ac_libsmdev_have_libuna=no])
	])

AS_IF(
	[test "x$ac_libsmdev_have_libuna" = xyes],
	[AC_SUBST(
	 [libsmdev_pc_libs_private_libuna],
	 [-luna])
	AC_SUBST(
	 [libsmdev_spec_requires],
	 [Requires:])
	AC_SUBST(
	 [libsmdev_spec_requires_libuna],
	 [libuna])
	AC_SUBST(
	 [libsmdev_spec_build_requires],
	 [BuildRequires:])
	AC_SUBST(
	 [libsmdev_spec_build_requires_libuna],
	 [libuna-devel]) ],
	[dnl If libuna is not available use the (local) version that came with libsmdev
	AC_DEFINE(
	 [HAVE_LOCAL_LIBUNA],
	 [1],
	 [Define to 1 if the local version of libuna is used.])
	AC_SUBST(
	 [HAVE_LOCAL_LIBUNA],
	 [1])
	AC_SUBST(
	 [libsmdev_pc_libs_private_libuna],
	 [])
	AC_SUBST(
	 [libsmdev_spec_requires],
	 [])
	AC_SUBST(
	 [libsmdev_spec_requires_libuna],
	 [])
	AC_SUBST(
	 [libsmdev_spec_build_requires],
	 [])
	AC_SUBST(
	 [libsmdev_spec_build_requires_libuna],
	 [])
	AC_SUBST(
	 [LIBUNA_CPPFLAGS],
	 [-I../libuna])
	AC_SUBST(
	 [LIBUNA_LIBADD],
	 [../libuna/libuna.la])
	ac_libsmdev_have_libuna=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBUNA],
 [test "x$ac_libsmdev_have_libuna" = xlocal])

dnl Check if libsmdev should be build with wide character type support
AS_IF(
	[test "x$ac_cv_libsmdev_enable_wide_character_type" != xno ],
	[dnl Check for the wide character header
	AC_CHECK_HEADERS(
	 [wchar.h],
	 [AC_DEFINE(
	  [HAVE_WCHAR_H],
	  [1],
	  [Define to 1 if you have the <wchar.h> header file.])
	 AC_SUBST(
	  [HAVE_WCHAR_H],
	  [1]) ],
	 [AC_MSG_FAILURE(
	  [Missing header: wchar.h],
	  [1]) ])

	dnl Determine wide character byte size
	AC_CHECK_SIZEOF([wchar_t])

	dnl Check for wide character string functions
	dnl Wide character string functions used in common/string_conversion.h
	AC_CHECK_FUNCS(
	 [btowc],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: btowc],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wctob],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: wctob],
	  [1]) ])

	dnl Wide character string functions used in common/wide_string.h
	AC_CHECK_FUNCS(
	 [fgetws],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: fgetws],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [swprintf],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: swprintf],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [vswprintf],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing function: vswprintf],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wcschr],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemchr],
	  [],
	  [AC_MSG_FAILURE(
	   [Missing functions: wcschr and wmemchr],
	   [1]) ]) ])

	AC_CHECK_FUNCS(
	 [wcsncasecmp],
	 [],
	 [AC_CHECK_FUNCS(
	  [wcscasecmp],
	  [],
	  [AC_MSG_FAILURE(
	   [Missing functions: wcsncasecmp and wcscasecmp],
	   [1]) ]) ])

	AC_CHECK_FUNCS(
	 [wcsncmp],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemcmp],
	  [],
	  [AC_CHECK_FUNCS(
	   [wcscmp],
	   [],
	   [AC_MSG_FAILURE(
	    [Missing functions: wcsncmp, wmemcmp and wcscmp],
	    [1]) ]) ]) ])

	AC_CHECK_FUNCS(
	 [wcsncpy],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemcpy],
	  [],
	  [AC_CHECK_FUNCS(
	   [wcscpy],
	   [],
	   [AC_MSG_FAILURE(
	    [Missing functions: wcsncpy, wmemcpy and wcscpy],
	    [1]) ]) ]) ])

	AC_CHECK_FUNCS(
	 [wcslen],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: wcslen],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wcsrchr],
	 [],
	 [AC_CHECK_FUNCS(
	  [wmemrchr],
	  [],
	  [AC_MSG_FAILURE(
	   [Missing functions: wcsrchr and wmemrchr],
	   [1]) ]) ])

	AC_CHECK_FUNCS(
	 [wcstoll],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: wcstoll],
	  [1]) ])

	AC_CHECK_FUNCS(
	 [wcstoull],
	 [],
	 [AC_MSG_FAILURE(
	  [Missing functions: wcstoull],
	  [1]) ])

	dnl Make the necessary definitions
	AC_DEFINE(
	 [HAVE_WIDE_CHARACTER_TYPE],
	 [1],
	 [Define to 1 if wide character type should be used.])
	AC_SUBST(
	 [HAVE_WIDE_CHARACTER_TYPE],
	 [1])
	ac_cv_libsmdev_enable_wide_character_type=yes],

	[AC_SUBST(
	 [HAVE_WIDE_CHARACTER_TYPE],
	 [0])
	AC_SUBST(
	 [HAVE_WCHAR_H],
	 [0]) ])

AC_SUBST(
 [HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS],
 [0])

dnl Check if libsmdev should be build with verbose output
AS_IF(
	[test "x$ac_cv_libsmdev_enable_verbose_output" != xno],
	[AC_DEFINE(
	 [HAVE_VERBOSE_OUTPUT],
	 [1],
	 [Define to 1 if verbose output should be used] )
	ac_cv_libsmdev_enable_verbose_output=yes])

dnl Check if libsmdev should be build with debug output
AS_IF(
	[test "x$ac_cv_libsmdev_enable_debug_output" != xno],
	[AC_DEFINE(
	 [HAVE_DEBUG_OUTPUT],
	 [1],
	 [Define to 1 if debug output should be used] )
	ac_cv_libsmdev_enable_debug_output=yes])

dnl Set memwatch support
ac_libsmdev_have_memwatch=yes

AS_IF(
	[test "x$ac_libsmdev_have_memwatch" != xno ],
	[AC_SUBST(
	 [MEMWATCH_CPPFLAGS],
	 ["-DMEMWATCH -DMEMWATCH_STDIO"])
	AC_SUBST(
	 [MEMWATCH_LIBADD],
	 [../common/libmemwatch.la]) ])

AM_CONDITIONAL(
 [HAVE_MEMWATCH],
 [test "x$ac_libsmdev_have_memwatch" != xno])

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if DLL support is needed in smdevtools
AS_IF(
	[test x"$enable_shared" = xyes],
	[AS_CASE(
		[$host],
		[*cygwin* | *mingw*],
		[AC_SUBST(
		 [LIBSMDEV_DLL_IMPORT],
		 ["-DLIBSMDEV_DLL_IMPORT"])
		])
	])

dnl Have configure make the Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([liberror/Makefile])
AC_CONFIG_FILES([libnotify/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libsmdev/Makefile])
AC_CONFIG_FILES([manuals/Makefile])
dnl Have configure make the include files
AC_CONFIG_FILES([include/libsmdev.h])
AC_CONFIG_FILES([include/libsmdev/definitions.h])
AC_CONFIG_FILES([include/libsmdev/features.h])
AC_CONFIG_FILES([include/libsmdev/types.h])
AC_CONFIG_FILES([libsmdev/libsmdev_definitions.h])
dnl Have configure make distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([libsmdev.pc])
AC_CONFIG_FILES([libsmdev.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libuna support:                             $ac_libsmdev_have_libuna

Features:
   Wide character type support:                $ac_cv_libsmdev_enable_wide_character_type
   Verbose output:                             $ac_cv_libsmdev_enable_verbose_output
   Debug output:                               $ac_cv_libsmdev_enable_debug_output
]);


