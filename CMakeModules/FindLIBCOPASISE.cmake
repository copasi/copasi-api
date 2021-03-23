# BEGIN: Copyright 
# Copyright (C) 2019 - 2021 Rector and Visitors of the University of Virginia 
# All rights reserved 
# END: Copyright 

# BEGIN: License 
# Licensed under the Apache License, Version 2.0 (the "License"); 
# you may not use this file except in compliance with the License. 
# You may obtain a copy of the License at 
#   http://www.apache.org/licenses/LICENSE-2.0 
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

message(STATUS "${CMAKE_INSTALL_LIBDIR}/cmake")

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

get_target_property(LIBCOPASISE_INTERFACE_LINK_LIBRARIES libCOPASISE-static INTERFACE_LINK_LIBRARIES)

if (LIBCOPASISE_INTERFACE_LINK_LIBRARIES)
  set(LIBCOPASISE_LIBRARY ${LIBCOPASISE_LIBRARY} ${LIBCOPASISE_INTERFACE_LINK_LIBRARIES})
endif (LIBCOPASISE_INTERFACE_LINK_LIBRARIES)


include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CROSSGUID REQUIRED LIBCOPASISE_INCLUDE_DIR LIBCOPASISE_LIBRARY)

mark_as_advanced(LIBCOPASISE_INCLUDE_DIR LIBCOPASISE_LIBRARY)

