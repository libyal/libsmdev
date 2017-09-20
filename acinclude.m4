dnl Checks for required headers and functions
dnl
dnl Version: 20170920

dnl Check if winioctl.h defines STORAGE_BUS_TYPE
AC_DEFUN([AX_LIBSMDEV_CHECK_HEADER_WINIOCTL_H_STORAGE_BUS_TYPE],
  [AC_CACHE_CHECK(
    [whether winioctl.h defines STORAGE_BUS_TYPE],
    [ac_cv_header_winioctl_h_storage_bus_type],
    [AC_LANG_PUSH(C)
    AC_COMPILE_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <windows.h>
#include <winioctl.h>]],
        [[STORAGE_BUS_TYPE storage_bus_type;
storage_bus_type = BusTypeUnknown;]] )],
      [ac_cv_header_winioctl_h_storage_bus_type=yes],
      [ac_cv_header_winioctl_h_storage_bus_type=no])
    AC_LANG_POP(C)],
    [ac_cv_header_winioctl_h_storage_bus_type=no])

  AS_IF(
    [test "x$ac_cv_header_winioctl_h_storage_bus_type" = xyes],
    [AC_DEFINE(
      [HAVE_WINIOCTL_H_STORAGE_BUS_TYPE],
      [1],
      [Define to 1 if STORAGE_BUS_TYPE is defined.])
    ])
  ])

dnl Function to detect if libsmdev dependencies are available
AC_DEFUN([AX_LIBSMDEV_CHECK_LOCAL],
  [dnl Headers included in libsmdev/libsmdev_handle.c and libsmdev/libsmdev_support.c
  AC_CHECK_HEADERS([errno.h sys/stat.h unistd.h])

  dnl Headers included in libsmdev/libsmdev_metadata.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_HEADERS([cygwin/fs.h linux/fs.h sys/disk.h sys/disklabel.h])
  ])

  dnl Headers included in libsmdev/libsmdev_ata.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_HEADERS([cygwin/hdreg.h linux/hdreg.h])
  ])

  dnl Headers included in libsmdev/libsmdev_scsi.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_HEADERS([scsi/scsi.h scsi/scsi_ioctl.h scsi/sg.h])
  ])

  dnl Headers included in libsmdev/libsmdev_optical_disk.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_HEADERS([linux/cdrom.h])
  ])

  dnl Headers included in libsmdev/libsmdev_usb.c
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xno],
    [AC_CHECK_HEADERS([linux/usbdevice_fs.h linux/usb/ch9.h sys/ioctl.h])
  ])

  dnl File input/output functions used in libbfio/libbfio_file.h
  AC_CHECK_FUNCS([close fstat ftruncate lseek open read stat write])

  AS_IF(
    [test "x$ac_cv_func_close" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: close],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_fstat" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: fstat],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_ftruncate" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: ftruncate],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_lseek" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: lseek],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_open" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: open],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_read" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: read],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_stat" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: stat],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_func_write" != xyes],
    [AC_MSG_FAILURE(
      [Missing function: write],
      [1])
    ])

  dnl Check for error string functions used in libsmdev/libsmdev_error_string.c
  AC_FUNC_STRERROR_R()

  AS_IF(
    [test "x$ac_cv_have_decl_strerror_r" = xno],
    [AC_CHECK_FUNCS([strerror])

    AS_IF(
      [test "x$ac_cv_func_strerror" != xyes],
      [AC_MSG_FAILURE(
        [Missing functions: strerror and strerror_r],
        [1])
      ])
    ])

  dnl Check if winioctl.h defines STORAGE_BUS_TYPE
  AS_IF(
    [test "x$ac_cv_enable_winapi" = xyes],
    [AX_LIBSMDEV_CHECK_HEADER_WINIOCTL_H_STORAGE_BUS_TYPE])

  dnl Check for internationalization functions in libsmdev/libsmdev_i18n.c
  AC_CHECK_FUNCS([bindtextdomain])

  dnl Check if library should be build with verbose output
  AX_COMMON_CHECK_ENABLE_VERBOSE_OUTPUT

  dnl Check if library should be build with debug output
  AX_COMMON_CHECK_ENABLE_DEBUG_OUTPUT

  dnl Check if DLL support is needed
  AS_IF(
    [test "x$enable_shared" = xyes],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])
    ])
  ])
])

dnl Function to detect if smdevtools dependencies are available
AC_DEFUN([AX_SMDEVTOOLS_CHECK_LOCAL],
  [AC_CHECK_HEADERS([signal.h sys/signal.h unistd.h])

  AC_CHECK_FUNCS([close getopt setvbuf])

  AS_IF(
   [test "x$ac_cv_func_close" != xyes],
   [AC_MSG_FAILURE(
     [Missing function: close],
     [1])
  ])

  dnl Check if tools should be build as static executables
  AX_COMMON_CHECK_ENABLE_STATIC_EXECUTABLES

  dnl Check if DLL support is needed
  AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw*],
      [AC_SUBST(
        [LIBSMDEV_DLL_IMPORT],
        ["-DLIBSMDEV_DLL_IMPORT"])
    ])
  ])
])
