#
# Copyright 2007,2008 by IDIAP Research Institute
#                        http://www.idiap.ch
#
# See the file COPYING for the licence associated with this software.
#

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([Tracter-GPL], [0.1.0])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([src/FourierFFTW.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

# Check for Tracter
PKG_CHECK_MODULES(
    TRACTER, tracter >= 0.2.0, ac_cv_tracter=1, ac_cv_tracter=0
)
AC_SUBST(TRACTER_CFLAGS)
AC_SUBST(TRACTER_LIBS)
AS_IF(
  [test "$ac_cv_tracter" = 0],
  [AC_ERROR([Required library TRACTER not found])]
)

# fftw3f
PKG_CHECK_MODULES(FFTW3F, fftw3f >= 3.1.2, ac_cv_fftw3f=1, ac_cv_fftw3f=0)
AS_IF(
  [test "$ac_cv_fftw3f" = 1],
  [AC_DEFINE([HAVE_FFTW3F], [1], [Define if you have libfftw3f.])]
)
AM_CONDITIONAL([WITH_FFTW3F], [test "$ac_cv_fftw3f" = 1])
AC_SUBST(FFTW3F_CFLAGS)
AC_SUBST(FFTW3F_LIBS)

# libsamplerate, from the FAQ at http://www.mega-nerd.com/SRC/faq.html
PKG_CHECK_MODULES(
    SAMPLERATE, samplerate >= 0.1.2, ac_cv_samplerate=1, ac_cv_samplerate=0
)
AS_IF(
  [test "$ac_cv_samplerate" = 1],
  [AC_DEFINE([HAVE_SAMPLERATE], [1], [Define if you have libsamplerate.])]
 )
AM_CONDITIONAL([WITH_SAMPLERATE], [test "$ac_cv_samplerate" = 1])
AC_SUBST(SAMPLERATE_CFLAGS)
AC_SUBST(SAMPLERATE_LIBS)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

AC_CONFIG_FILES(
	[Makefile
	 tracter-gpl.pc
	 Doxyfile
	 doc/Makefile
	 src/Makefile]
)
AC_OUTPUT
