AC_INIT([libmmngrbuf], [1.0.0])
AC_CONFIG_SRCDIR([if/mmngr_buf_if.c])
AM_INIT_AUTOMAKE
AC_PREREQ(2.59)
AC_PROG_LIBTOOL

AC_PROG_CC
AC_PROG_INSTALL

AC_HEADER_STDC

AC_ARG_ENABLE([mmngrbuf-test], [AC_HELP_STRING([--enable-mmngrbuf-test@<:@=yes/no@:>@],\
    [Enable MMNGRBUF test @<:@default=no@:>@])], [mmngrbuf_test=yes], [mmngrbuf_test=no])
AM_CONDITIONAL([HAVE_MMNGRBUF_TEST], [test "x${mmngrbuf_test}" = "xyes"])

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(if/Makefile)
AC_CONFIG_FILES(tp/Makefile)
AC_CONFIG_MACRO_DIRS([m4])
AC_OUTPUT
