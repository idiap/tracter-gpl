#
# CMake file for Tracter GPL
#
# Phil Garner
# March 2010
#

# Version 2.4 might work, but it won't find static libraries with odd names
cmake_minimum_required(VERSION 2.6)

# This project's name
project(tracter-gpl)
set(VERSION 1.0)

# Find packages
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Tracter)
find_package(Samplerate)
find_package(FFTW3F)

add_subdirectory(src)

# Configure other files
configure_file(Doxyfile.in Doxyfile)

# This one is for binary distributions
include(CPack)

# For source, it's *much* better to use git
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${VERSION})
add_custom_target(dist
  COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
    | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
