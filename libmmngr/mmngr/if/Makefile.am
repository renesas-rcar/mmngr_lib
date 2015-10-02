lib_LTLIBRARIES = libmmngr.la

AM_CFLAGS = -I${top_srcdir}/include

include_HEADERS = $(top_srcdir)/include/mmngr_user_private.h \
                  $(top_srcdir)/include/mmngr_user_public.h

libmmngr_la_LDFLAGS = -version-info 1:0:0
libmmngr_la_SOURCES = mmngr_if.c
