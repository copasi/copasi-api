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

# if git is present, get the version string from it
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  
	execute_process(
	COMMAND "${GIT_EXECUTABLE}"
		 describe --always --dirty
	OUTPUT_VARIABLE GIT_COMMIT_OUT
	OUTPUT_STRIP_TRAILING_WHITESPACE
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
else()
	# otherwise set it to unkwon
	set (GIT_COMMIT_OUT "unknown")
endif()

# set the define string
set (GIT_COMMIT_OUT "#define GIT_COMMIT \"${GIT_COMMIT_OUT}\"")

# read existing string from file
set (CURRENT)
if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h)
	file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h CURRENT)
endif()

# DEBUG
# message( "
# 
# 	GIT_COMMIT_OUT: ${GIT_COMMIT_OUT}
# 	CURRENT: ${CURRENT}
# 	CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}
# ")

if (NOT CURRENT EQUAL GIT_COMMIT_OUT)
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h ${GIT_COMMIT_OUT})
else()
	message("No Changes to Git Version")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h)

set(CPSAPI_SOURCES)
set(CPSAPI_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h)

include(core/CMakeLists.txt)
include(model/CMakeLists.txt)

add_library (cpsapi STATIC ${CPSAPI_SOURCES} ${CPSAPI_HEADERS})
target_link_libraries(cpsapi ${LIBCOPASISE_LIBRARY_NAME})

# set include directory, so that binary dir will be included during build
target_include_directories(cpsapi PUBLIC
 
   $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
   $<INSTALL_INTERFACE:cpsapi> 
)

if (OPENMP_ENABLED)
	target_compile_options(cpsapi PUBLIC ${OpenMP_CXX_FLAGS})
endif (OPENMP_ENABLED)
