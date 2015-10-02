lib_LTLIBRARIES = libmmngrbuf.la

AM_CFLAGS = -I${top_srcdir}/include

include_HEADERS = $(top_srcdir)/include/mmngr_buf_user_private.h \
                  $(top_srcdir)/include/mmngr_buf_user_public.h

libmmngrbuf_la_LDFLAGS = -version-info 1:0:0
libmmngrbuf_la_SOURCES = mmngr_buf_if.c
