AC_INIT([libmmngr], [1.0.0])
AC_CONFIG_SRCDIR([if/mmngr_if.c])
AM_INIT_AUTOMAKE
AC_PREREQ(2.59)
AC_PROG_LIBTOOL

AC_PROG_CC
AC_PROG_INSTALL

AC_HEADER_STDC

AC_ARG_ENABLE([mmngr-test], [AC_HELP_STRING([--enable-mmngr-test@<:@=yes/no@:>@],\
    [Enable MMNGR test @<:@default=no@:>@])], [mmngr_test=yes], [mmngr_test=no])
AM_CONDITIONAL([HAVE_MMNGR_TEST], [test "x${mmngr_test}" = "xyes"])

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(if/Makefile)
AC_CONFIG_FILES(tp/Makefile)
AC_CONFIG_MACRO_DIRS([m4])
AC_OUTPUT
