#
# Copyright 2007,2008 by IDIAP Research Institute
#                        http://www.idiap.ch
#
# See the file COPYING for the licence associated with this software.
#

lib_LTLIBRARIES = libtracter-gpl.la

TRACTER_GPL_SOURCES =

pkginclude_HEADERS = $(TRACTER_GPL_SOURCES:.cpp=.h)

libtracter_gpl_la_SOURCES = $(TRACTER_GPL_SOURCES)

# Blank, but added to later
nodist_libtracter_gpl_la_SOURCES =
libtracter_gpl_la_CPPFLAGS = @TRACTER_CFLAGS@
libtracter_gpl_la_CFLAGS =

if WITH_FFTW3F
libtracter_gpl_la_SOURCES += FourierFFTW.cpp
libtracter_gpl_la_CFLAGS += @FFTW3F_CFLAGS@
endif

if WITH_SAMPLERATE
TRACTER_GPL_SOURCES += Resample.cpp
libtracter_gpl_la_CFLAGS += @SAMPLERATE_CFLAGS@
endif

libtracter_gpl_la_LIBADD = @TRACTER_LIBS@
