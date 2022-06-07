# BEGIN: Copyright 
# Copyright (C) 2021 - 2022 by Pedro Mendes, Rector and Visitors of the 
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

cmake_minimum_required (VERSION 3.1.3)

project(CPSAPI VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CYGWIN)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()


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

# compile as position independent code
if (UNIX OR CYGWIN)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set(CPSAPI_DEPENDENCY_DIR "" CACHE PATH "Installation directory of the CPSAPI dependencies")

if (CPSAPI_DEPENDENCY_DIR)
  include_directories(BEFORE ${CPSAPI_DEPENDENCY_DIR}/include)
  add_link_options("-L${CPSAPI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}")
endif (CPSAPI_DEPENDENCY_DIR)

message (STATUS "COPASI dependencies: " ${CPSAPI_DEPENDENCY_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake)

option (ENABLE_CPP_NAMESPACE "Enable the use of cpsapi namespace." ON)

if (ENABLE_CPP_NAMESPACE)
  set(CPSAPI_USE_CPP_NAMESPACE 1)
endif (ENABLE_CPP_NAMESPACE)

find_package(LIBCOPASISE REQUIRED)
include_directories(BEFORE ${LIBCOPASISE_INCLUDE_DIR})

add_subdirectory(cpsapi)

option (WITH_DOCUMENTATION "Build the documentation." OFF)
if (WITH_DOCUMENTATION)
add_subdirectory (docs)
endif()

add_subdirectory(examples)

include(CTest)
add_subdirectory(tests)

# for the language bindings we need swig
option (ENABLE_CSHARP "Enable language bindings for the C# language." OFF)
option (ENABLE_JAVA "Enable language bindings for the Java language." OFF)
option (ENABLE_PYTHON "Enable language bindings for the Python language." OFF)
option (ENABLE_R "Enable language bindings for the R language." OFF)

# we need swig for the language bindings. Only if SWIG was found, the options to
# build the different language bindings are enabled
# TODO test the SWIG version
if (ENABLE_CSHARP OR ENABLE_JAVA OR ENABLE_PYTHON OR ENABLE_R)
  add_subdirectory(bindings)
endif ()

get_directory_property(DirDefs COMPILE_DEFINITIONS)

message(STATUS "-----------------------------------------------------------
 Building ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_VERSION} (${CMAKE_BUILD_TYPE})

   Source Dir             = ${CMAKE_SOURCE_DIR}
   Binary Dir             = ${CMAKE_BINARY_DIR}
   Install Prefix         = ${CMAKE_INSTALL_PREFIX}

 Dependencies:
   libCOPASISE include    = ${LIBCOPASISE_INCLUDE_DIR}
   libCOPASISE Libs       = ${LIBCOPASISE_LIBRARY}
   libCOPASISE LDFLAGS    = ${LIBCOPASISE_LINKER_FLAGS}

   COPASI dependencies    = ${CPSAPI_DEPENDENCY_DIR}

 Options:

   Enable namespace       = ${ENABLE_CPP_NAMESPACE}
   Generate documentation = ${WITH_DOCUMENTATION}
   Additional Defines     = ${DirDefs}
")
