#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(ruok, 0.15, radubrumariu@gmail.com)
AC_CONFIG_MACRO_DIR([m4])

PKG_PROG_PKG_CONFIG
AM_INIT_AUTOMAKE(ruok, 2.6)

AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL 

# Checks for libraries.
LIBCURL_CHECK_CONFIG
PKG_CHECK_MODULES([libxml2],[libxml-2.0])

# Checks for header files.

AC_CHECK_HEADERS([getopt.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CHECK_FUNCS([getopt_long],AC_DEFINE(HAVE_GETOPT_LONG))

AC_OUTPUT([Makefile src/Makefile])
