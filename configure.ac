#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(ruok, 0.2, radubrumariu@gmail.com)
AC_CONFIG_MACRO_DIR([m4])

PKG_PROG_PKG_CONFIG
AM_INIT_AUTOMAKE([1.11.1 -Wall])

AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([ Makefile
		  libs/Makefile
		  src/Makefile])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_YACC
AM_PROG_LEX
AC_PROG_INSTALL 
AC_PROG_LIBTOOL 
AM_PROG_CC_C_O

# Checks for libraries.
PKG_CHECK_MODULES([libxml2],[libxml-2.0])
PKG_CHECK_MODULES([libcurl],[libcurl])
# Checks for header files.

AC_CHECK_HEADERS([getopt.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CHECK_FUNCS([getopt_long],AC_DEFINE(HAVE_GETOPT_LONG))
AC_OUTPUT()
