# BEGIN: Copyright 
# Copyright (C) 2021 Rector and Visitors of the University of Virginia 
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

find_package(LIBCOPASISE REQUIRED)
include_directories(BEFORE ${LIBCOPASISE_INCLUDE_DIR} "/home/shoops/workspace/copasi-dependencies/local/include")

add_subdirectory(cpsapi)

get_directory_property(DirDefs COMPILE_DEFINITIONS)

message(STATUS "-----------------------------------------------------------
 Building CPSAPI ${COPASI_VERSION_MAJOR}.${COPASI_VERSION_MINOR}.${COPASI_VERSION_BUILD} (${CMAKE_BUILD_TYPE})

   Source Dir           = ${CMAKE_SOURCE_DIR}
   Binary Dir           = ${CMAKE_BINARY_DIR}
   Install Prefix       = ${CMAKE_INSTALL_PREFIX}

 Dependencies:
   libCOPASISE include = ${LIBCOPASISE_INCLUDE_DIR}
   libCOPASISE Libs    = ${LIBCOPASISE_LIBRARY}
   libCOPASISE LDFLAGS  = ${LIBCOPASISE_LINKER_FLAGS}
")
