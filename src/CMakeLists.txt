# Tracter is compulsory
include_directories(${TRACTER_INCLUDE_DIRS})
link_directories(${TRACTER_LIBRARY_DIRS})
list(APPEND PKGCONFIG_REQUIRES tracter)

# FFTW
if(${FFTW3F_FOUND})
  list(APPEND SOURCES FourierFFTW.cpp)
  add_executable(fft fft.cpp)
  target_link_libraries(fft static-lib)
  include_directories(${FFTW3F_INCLUDE_DIRS})
  list(APPEND PKGCONFIG_REQUIRES fftw3f)
endif(${FFTW3F_FOUND})

# SRC
if(${SAMPLERATE_FOUND})
  list(APPEND SOURCES ResampleSRC.cpp)
  add_executable(resample resample.cpp)
  target_link_libraries(resample static-lib)
  include_directories(${SAMPLERATE_INCLUDE_DIRS})
  list(APPEND PKGCONFIG_REQUIRES samplerate)
endif(${SAMPLERATE_FOUND})

# Things to install
set(INSTALL_TARGETS
  static-lib
)

# A little wordy, but allows static lib also with the same syntax
set(TARGET_LIBS
  ${TRACTER_LIBRARIES}
  ${FFTW3F_LIBRARIES}
  ${SAMPLERATE_LIBRARIES}
)

# Static library
add_library(static-lib STATIC ${SOURCES})
set_target_properties(static-lib
  PROPERTIES OUTPUT_NAME "tracter-gpl"
)
target_link_libraries(static-lib ${TARGET_LIBS})

# Shared library
set(USE_SHARED $ENV{USE_SHARED}
  CACHE BOOLEAN "Whether to build shared library"
  )
if (USE_SHARED)
  add_library(shared-lib SHARED ${SOURCES})
  set_target_properties(shared-lib
    PROPERTIES OUTPUT_NAME "tracter-gpl"
    )
  target_link_libraries(shared-lib ${TARGET_LIBS})
  list(APPEND INSTALL_TARGETS shared-lib)
endif (USE_SHARED)

install(
  TARGETS ${INSTALL_TARGETS}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include/tracter
  )

# pkgconfig install lines
string(REGEX REPLACE ";" "," TRACTER_GPL_REQUIRES "${PKGCONFIG_REQUIRES}")
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix "\${prefix}")
set(libdir "\${exec_prefix}/lib")
set(includedir "\${prefix}/include/tracter")
configure_file(tracter-gpl.pc.in ${CMAKE_CURRENT_BINARY_DIR}/tracter-gpl.pc)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/tracter-gpl.pc
  DESTINATION lib/pkgconfig
  )
