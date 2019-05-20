# Copyright (C) 2018 by Pedro Mendes, Virginia Tech Intellectual 
# Properties, Inc., University of Heidelberg, and University of 
# of Connecticut School of Medicine. 
# All rights reserved. 




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
  CONFIGS libcopasise-config.cmake
  PATHS $ENV{LIBCOPASISE_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake
        ${COPASI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake
        /usr/${CMAKE_INSTALL_LIBDIR}/cmake
  )

set(LIBCOPASISE_FOUND ${libcopasise_FOUND})

get_target_property(LIBCOPASISE_INCLUDE_DIR libcopasise INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(LIBCOPASISE_LIBRARY libcopasise IMPORTED_LOCATION_RELEASE)

if (NOT LIBCOPASISE_LIBRARY)
  get_target_property(LIBCOPASISE_LIBRARY libcopasise IMPORTED_LOCATION_DEBUG)
endif()

get_target_property(LIBCOPASISE_INTERFACE_LINK_LIBRARIES libcopasise INTERFACE_LINK_LIBRARIES)

if (LIBCOPASISE_INTERFACE_LINK_LIBRARIES)
  set(LIBCOPASISE_LIBRARY ${LIBCOPASISE_LIBRARY} ${LIBCOPASISE_INTERFACE_LINK_LIBRARIES})
endif (LIBCOPASISE_INTERFACE_LINK_LIBRARIES)


include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CROSSGUID REQUIRED LIBCOPASISE_INCLUDE_DIR LIBCOPASISE_LIBRARY)

mark_as_advanced(LIBCOPASISE_INCLUDE_DIR LIBCOPASISE_LIBRARY)

