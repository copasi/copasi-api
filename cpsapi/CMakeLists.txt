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

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  add_custom_target(GIT_COMMIT 
  	COMMAND echo "\\#define GIT_COMMIT" \\\"`${GIT_EXECUTABLE} describe --always --dirty`\\\" > ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h
    COMMAND if [ ! -e git-commit.h ] || ! diff --brief git-commit.h ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h &> /dev/null\; then cp ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h git-commit.h\; fi
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
  add_custom_target(GIT_COMMIT 
  	COMMAND echo "\\#define GIT_COMMIT" \\\"unknown\\\" > ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h
    COMMAND if [ ! -e git-commit.h ] || ! diff --brief git-commit.h ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h &> /dev/null\; then cp ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h git-commit.h\; fi
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h)

set(CPSAPI_SOURCES)
set(CPSAPI_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h)

include(core/CMakeLists.txt)
include(model/CMakeLists.txt)

add_library (cpsapi STATIC ${CPSAPI_SOURCES} ${CPSAPI_HEADERS})

add_dependencies(cpsapi GIT_COMMIT)