AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libnotify \
	@LIBUNA_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

lib_LTLIBRARIES = libsmdev.la

libsmdev_la_SOURCES = \
	libsmdev.c \
	libsmdev_ata.c libsmdev_ata.h \
	libsmdev_codepage.h \
	libsmdev_definitions.h \
	libsmdev_error.c libsmdev_error.h \
	libsmdev_error_string.c libsmdev_error_string.h \
	libsmdev_extern.h \
	libsmdev_handle.c libsmdev_handle.h \
	libsmdev_libuna.h \
	libsmdev_list_type.c libsmdev_list_type.h \
	libsmdev_metadata.c libsmdev_metadata.h \
	libsmdev_notify.c libsmdev_notify.h \
	libsmdev_offset_list.c libsmdev_offset_list.h \
	libsmdev_optical_disk.c libsmdev_optical_disk.h \
	libsmdev_scsi.c libsmdev_scsi.h \
	libsmdev_string.c libsmdev_string.h \
	libsmdev_support.c libsmdev_support.h \
	libsmdev_system_string.c libsmdev_system_string.h \
	libsmdev_types.h \
	libsmdev_usb.c libsmdev_usb.h

libsmdev_la_LIBADD = \
	../liberror/liberror.la \
	../libnotify/libnotify.la \
	@LIBUNA_LIBADD@

libsmdev_la_LDFLAGS = -no-undefined -version-info 1:0:0

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libsmdev ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libsmdev_la_SOURCES) 

