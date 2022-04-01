# BEGIN: Copyright 
# Copyright (C) 2019 - 2022 by Pedro Mendes, Rector and Visitors of the 
# University of Virginia, University of Heidelberg, and University 
# of Connecticut School of Medicine. 
# All rights reserved 
# END: Copyright 

# BEGIN: License 
# Licensed under the Artistic License 2.0 (the "License"); 
# you may not use this file except in compliance with the License. 
# You may obtain a copy of the License at 
#   https://opensource.org/licenses/Artistic-2.0 
# END: License 

# Locate library providing libcopasise
# This module defines:
# LIBCOPASISE_INCLUDE_DIR, where to find the headers
#
# LIBCOPASISE, LIBCOPASISE_DEBUG
# LIBCOPASISE_FOUND
#
# $LIBCOPASISE_DIR is an environment variable that would
# correspond to the ./configure --prefix=$LIBCOPASISE_DIR

find_package(libcopasise CONFIG REQUIRED
  CONFIGS libcopasise-static-config.cmake
  PATHS $ENV{LIBCOPASISE_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake
        ${COPASI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake
        /usr/${CMAKE_INSTALL_LIBDIR}/cmake
  )

set(LIBCOPASISE_FOUND ${libcopasise_FOUND})

get_target_property(LIBCOPASISE_INCLUDE_DIR libCOPASISE-static INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(LIBCOPASISE_LIBRARY libCOPASISE-static IMPORTED_LOCATION_RELEASE)

if (NOT LIBCOPASISE_LIBRARY)
  get_target_property(LIBCOPASISE_LIBRARY libCOPASISE-static IMPORTED_LOCATION_DEBUG)
endif()

if (NOT LIBCOPASISE_LIBRARY)
  get_target_property(LIBCOPASISE_LIBRARY libCOPASISE-static IMPORTED_LOCATION_DEBUG)
endif()


get_target_property(LIBCOPASISE_INTERFACE_LINK_LIBRARIES libCOPASISE-static INTERFACE_LINK_LIBRARIES)

if (LIBCOPASISE_INTERFACE_LINK_LIBRARIES)
  set(LIBCOPASISE_LIBRARY ${LIBCOPASISE_LIBRARY} ${LIBCOPASISE_INTERFACE_LINK_LIBRARIES})
endif (LIBCOPASISE_INTERFACE_LINK_LIBRARIES)

string(REPLACE include share/copasi/CMakeModules LIBCOPASISE_CMAKEMODULE_DIR ${LIBCOPASISE_INCLUDE_DIR})
set(CMAKE_MODULE_PATH ${LIBCOPASISE_CMAKEMODULE_DIR} ${CMAKE_MODULE_PATH})

foreach (library ${LIBCOPASISE_INTERFACE_LINK_LIBRARIES})
  string(FIND "${library}" "::" index)

  if (${index} GREATER 0)
    # found dependent library
    string(SUBSTRING "${library}" 0 ${index} DEPENDENT_NAME)
    message(VERBOSE "Looking for dependent library: ${DEPENDENT_NAME}")
    find_package(${DEPENDENT_NAME})
    set(LIBCOPASISE_INCLUDE_DIR ${LIBCOPASISE_INCLUDE_DIR} ${${DEPENDENT_NAME}_INCLUDE_DIR})
    endif()
endforeach()

list(FIND LIBCOPASISE_LIBRARY "OpenMP::OpenMP_CXX" OPENMP_ENABLED)

if (${OPENMP_ENABLED} GREATER -1)
  set(OPENMP_ENABLED ON)
else()
  set(OPENMP_ENABLED OFF) 
endif()

find_package(LIBSBML REQUIRED)
set(LIBCOPASISE_INCLUDE_DIR ${LIBCOPASISE_INCLUDE_DIR} ${LIBSBML_INCLUDE_DIR})
find_package(LIBSEDML REQUIRED)
set(LIBCOPASISE_INCLUDE_DIR ${LIBCOPASISE_INCLUDE_DIR} ${LIBSEDML_INCLUDE_DIR})
find_package(LIBCOMBINE REQUIRED)
set(LIBCOPASISE_INCLUDE_DIR ${LIBCOPASISE_INCLUDE_DIR} ${LIBCOMBINE_INCLUDE_DIR})

list(REMOVE_DUPLICATES LIBCOPASISE_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBCOPASISE REQUIRED LIBCOPASISE_LIBRARY LIBCOPASISE_INCLUDE_DIR)

mark_as_advanced(LIBCOPASISE_INCLUDE_DIR LIBCOPASISE_LIBRARY)

