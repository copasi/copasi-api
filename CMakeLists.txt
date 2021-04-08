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

cmake_minimum_required (VERSION 3.1.3)

project(CPSAPI VERSION "0.0.1")

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

set(EXTRA_DEFS "" CACHE STRING "additional definitions to be used." )
set(EXTRA_LIBS "" CACHE STRING "List of additional libraries to link against.  Separate multiple names using semicolons." )
set(EXTRA_INCLUDE_DIRS "" CACHE STRING "List of additional include directories to use during compilation. Separate multiple directories using semicolons." )

if(EXTRA_DEFS)
  add_definitions(${EXTRA_DEFS})
endif(EXTRA_DEFS)

if(EXTRA_INCLUDE_DIRS)
  foreach(dir ${EXTRA_INCLUDE_DIRS})
    include_directories(BEFORE ${dir})
  endforeach()
endif(EXTRA_INCLUDE_DIRS)

set(COPASI_DEPENDENCY_DIR "" CACHE PATH "Installation directory of the COPASI dependencies")

if (COPASI_DEPENDENCY_DIR)
  include_directories(BEFORE ${COPASI_DEPENDENCY_DIR}/include)
  add_link_options("-L${COPASI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}")
endif (COPASI_DEPENDENCY_DIR)

message (STATUS "COPASI dependencies: " ${COPASI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake)

option (ENABLE_CPP_NAMESPACE "Enable the use of cpsapi namespace." OFF)

if (ENABLE_CPP_NAMESPACE)
  set(CPSAPI_USE_CPP_NAMESPACE 1)
endif (ENABLE_CPP_NAMESPACE)

find_package(LIBCOPASISE REQUIRED)
include_directories(BEFORE ${LIBCOPASISE_INCLUDE_DIR})

add_subdirectory(cpsapi)
add_subdirectory(examples)

get_directory_property(DirDefs COMPILE_DEFINITIONS)

message(STATUS "-----------------------------------------------------------
 Building ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION} (${CMAKE_BUILD_TYPE})

   Source Dir           = ${CMAKE_SOURCE_DIR}
   Binary Dir           = ${CMAKE_BINARY_DIR}
   Install Prefix       = ${CMAKE_INSTALL_PREFIX}

 Dependencies:
   libCOPASISE include  = ${LIBCOPASISE_INCLUDE_DIR}
   libCOPASISE Libs     = ${LIBCOPASISE_LIBRARY}
   libCOPASISE LDFLAGS  = ${LIBCOPASISE_LINKER_FLAGS}

   COPASI dependencies  = ${COPASI_DEPENDENCY_DIR}

 Options:

   Enable namespace     = ${ENABLE_CPP_NAMESPACE}
   Additional Defines   = ${DirDefs}
")
