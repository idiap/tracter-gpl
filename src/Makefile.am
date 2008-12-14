#
# Copyright 2007,2008 by IDIAP Research Institute
#                        http://www.idiap.ch
#
# See the file COPYING for the licence associated with this software.
#

lib_LTLIBRARIES = libtracter-gpl.la

TRACTER_GPL_SOURCES =
noinst_PROGRAMS =

pkginclude_HEADERS = $(TRACTER_GPL_SOURCES:.cpp=.h)

libtracter_gpl_la_SOURCES = $(TRACTER_GPL_SOURCES)

# Blank, but added to later
nodist_libtracter_gpl_la_SOURCES =
libtracter_gpl_la_CPPFLAGS = @TRACTER_CFLAGS@

if WITH_FFTW3F
libtracter_gpl_la_SOURCES += FourierFFTW.cpp
libtracter_gpl_la_CPPFLAGS += @FFTW3F_CFLAGS@
noinst_PROGRAMS += fft
endif

if WITH_SAMPLERATE
libtracter_gpl_la_SOURCES += ResampleSRC.cpp
libtracter_gpl_la_CPPFLAGS += @SAMPLERATE_CFLAGS@
noinst_PROGRAMS += resample
endif

libtracter_gpl_la_LIBADD = \
	@FFTW3F_LIBS@ \
	@SAMPLERATE_LIBS@ \
	@TRACTER_LIBS@

resample_CPPFLAGS = @TRACTER_CFLAGS@ @SAMPLERATE_CFLAGS@
resample_SOURCES = resample.cpp
resample_LDADD = libtracter-gpl.la

fft_CPPFLAGS = @TRACTER_CFLAGS@ @FFTW3F_CFLAGS@
fft_SOURCES = fft.cpp
fft_LDADD = libtracter-gpl.la
