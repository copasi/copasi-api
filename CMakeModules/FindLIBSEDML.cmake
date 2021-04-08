# BEGIN: Copyright 
# Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
# University of Virginia, University of Heidelberg, and University 
# of Connecticut School of Medicine. 
# All rights reserved 
# END: Copyright 

# BEGIN: License 
# Licensed under the Apache License, Version 2.0 (the "License"); 
# you may not use this file except in compliance with the License. 
# You may obtain a copy of the License at 
#   http://www.apache.org/licenses/LICENSE-2.0 
# END: License 

# Copyright (C) 2019 by Pedro Mendes, Rector and Visitors of the 
# University of Virginia, University of Heidelberg, and University 
# of Connecticut School of Medicine. 
# All rights reserved. 

# Copyright (C) 2017 - 2018 by Pedro Mendes, Virginia Tech Intellectual 
# Properties, Inc., University of Heidelberg, and University of 
# of Connecticut School of Medicine. 
# All rights reserved. 

# Copyright (C) 2013 - 2016 by Pedro Mendes, Virginia Tech Intellectual 
# Properties, Inc., University of Heidelberg, and The University 
# of Manchester. 
# All rights reserved. 

# Locate libsedml
# This module defines:
# LIBSEDML_INCLUDE_DIR, where to find the headers
#
# LIBSEDML_LIBRARY,
# LIBSEDML_FOUND
#
# $LIBSEDML_DIR is an environment variable that would
# correspond to the ./configure --prefix=$LIBSEDML_DIR
#
# Created by Robert Osfield.
# Modified by Ralph Gauges


if(UNIX OR CYGWIN OR MINGW)
  set(LIBSEDML_LIBRARY_NAME sedml)
else()
  set(LIBSEDML_LIBRARY_NAME libsedml)
endif()

message (STATUS "Looking for ${LIBSEDML_LIBRARY_NAME}")

find_package(${LIBSEDML_LIBRARY_NAME} CONFIG QUIET)

if (NOT ${LIBSEDML_LIBRARY_NAME}_FOUND)
  find_package(${LIBSEDML_LIBRARY_NAME} CONFIG QUIET
    PATHS /usr/lib/cmake
          /usr/local/lib/cmake
          /opt/lib/cmake
          /opt/local/lib/cmake
          /sw/lib/cmake
          ${COPASI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake
          ${CONAN_LIB_DIRS_LIBSEDML}/cmake
  )
endif()

if (${LIBSEDML_LIBRARY_NAME}_FOUND)

  get_target_property(LIBSEDML_LIBRARY ${LIBSEDML_LIBRARY_NAME} LOCATION)
  get_filename_component(LIB_PATH ${LIBSEDML_LIBRARY} DIRECTORY)
  file(TO_CMAKE_PATH ${LIB_PATH}/../include LIBSEDML_INCLUDE_DIR)  
  get_filename_component (LIBSEDML_INCLUDE_DIR ${LIBSEDML_INCLUDE_DIR} REALPATH)
  get_target_property(LIBSEDML_VERSION ${LIBSEDML_LIBRARY_NAME} VERSION)

else()

find_path(LIBSEDML_INCLUDE_DIR sedml/SedBase.h
    PATHS $ENV{LIBSEDML_DIR}/include
          $ENV{LIBSEDML_DIR}
           ${COPASI_DEPENDENCY_DIR}/include
          ${COPASI_DEPENDENCY_DIR}
          ~/Library/Frameworks
          /Library/Frameworks
          /sw/include        # Fink
          /opt/local/include # MacPorts
          /opt/csw/include   # Blastwave
          /opt/include
          /usr/freeware/include
    NO_DEFAULT_PATH)

if (NOT LIBSEDML_INCLUDE_DIR)
    find_path(LIBSEDML_INCLUDE_DIR sedml/SedBase.h)
endif (NOT LIBSEDML_INCLUDE_DIR)

find_library(LIBSEDML_LIBRARY 
    NAMES sedml-static 
          sedml
          libsedml-static 
          libsedml
    PATHS $ENV{LIBSEDML_DIR}/lib
          $ENV{LIBSEDML_DIR}
          ${COPASI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}
          ${COPASI_DEPENDENCY_DIR}/lib
          ${COPASI_DEPENDENCY_DIR}
          ${CONAN_LIB_DIRS_LIBSEDML}
          ~/Library/Frameworks
          /Library/Frameworks
          /sw/lib        # Fink
          /opt/local/lib # MacPorts
          /opt/csw/lib   # Blastwave
          /opt/lib
          /usr/freeware/lib64
    NO_DEFAULT_PATH)

if (NOT LIBSEDML_LIBRARY)
    find_library(LIBSEDML_LIBRARY 
        NAMES sedml-static 
              sedml)
endif (NOT LIBSEDML_LIBRARY)

if (NOT LIBSEDML_LIBRARY)
    message(FATAL_ERROR "libSEDML library not found!")
endif (NOT LIBSEDML_LIBRARY)


find_library(LIBNUML_LIBRARY 
    NAMES numl-static 
          numl
          libnuml-static 
          libnuml
    PATHS $ENV{LIBSEDML_DIR}/lib
          $ENV{LIBSEDML_DIR}
          ${COPASI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}
          ${COPASI_DEPENDENCY_DIR}/lib
          ${COPASI_DEPENDENCY_DIR}
          ${CONAN_LIB_DIRS_LIBNUML}
          ~/Library/Frameworks
          /Library/Frameworks
          /sw/lib        # Fink
          /opt/local/lib # MacPorts
          /opt/csw/lib   # Blastwave
          /opt/lib
          /usr/freeware/lib64
    NO_DEFAULT_PATH)

endif()

  add_library(${LIBSEDML_LIBRARY_NAME} UNKNOWN IMPORTED)
  set_target_properties(${LIBSEDML_LIBRARY_NAME} PROPERTIES IMPORTED_LOCATION ${LIBSEDML_LIBRARY})
  set_target_properties(${LIBSEDML_LIBRARY_NAME} PROPERTIES INTERFACE_LINK_LIBRARIES ${LIBNUML_LIBRARY})
  


if (NOT LIBSEDML_INCLUDE_DIR)
    message(FATAL_ERROR "libsedml include dir not found not found!")
endif (NOT LIBSEDML_INCLUDE_DIR)

if (NOT LIBSEDML_LIBRARY)
    message(FATAL_ERROR "LIBSEDML library not found!")
endif (NOT LIBSEDML_LIBRARY)

set(LIBSEDML_FOUND "NO")
if(LIBSEDML_LIBRARY)
    if   (LIBSEDML_INCLUDE_DIR)
        SET(LIBSEDML_FOUND "YES")
    endif(LIBSEDML_INCLUDE_DIR)
endif(LIBSEDML_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set LIBSEDML_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBSEDML DEFAULT_MSG LIBSEDML_LIBRARY LIBSEDML_INCLUDE_DIR)

mark_as_advanced(LIBSEDML_INCLUDE_DIR LIBSEDML_LIBRARY LIBNUML_LIBRARY)
