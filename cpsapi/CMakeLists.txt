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

find_package(Git QUIET)

include (CMakePackageConfigHelpers)

###############################################################################
#
# utility macro for copying files only if they have changed, for this it is
# assumed that the source file is always created. Then the macro is called.
# If the target file will only be overwritten if it does not yet exist, or
# its content is different. At the end the source file will be removed.
#
macro(copy_if_different_and_remove source target)
# message(
# "
# copy_if_different_and_remove called with:
#     source = ${source}
#     target = ${target}
# "
# )
file(READ ${source} CONTENT)

if (EXISTS ${target})
    # message("target exists")
    file(READ ${target} CURRENT_CONTENT)
    string(COMPARE EQUAL ${CURRENT_CONTENT} ${CONTENT} IS_SAME)
    if (NOT ${IS_SAME})
        #message("content is different")
        file(WRITE ${target} ${CONTENT})
    endif()
else()
    #message("target does not exist")
    file(WRITE ${target} ${CONTENT})
endif()

file(REMOVE ${source})

endmacro(copy_if_different_and_remove)

###############################################################################


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
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/git-commit.h)
	file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/git-commit.h CURRENT)
endif()

# DEBUG
# message( "
# 
# 	GIT_COMMIT_OUT: ${GIT_COMMIT_OUT}
# 	CURRENT: ${CURRENT}
# 	CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_SOURCE_DIR}
# ")


file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h ${GIT_COMMIT_OUT})

copy_if_different_and_remove(
  ${CMAKE_CURRENT_BINARY_DIR}/git-commit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/git-commit.h
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h)

set(CPSAPI_SOURCES)
set(CPSAPI_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h)

include(core/CMakeLists.txt)
include(model/CMakeLists.txt)
include(task/CMakeLists.txt)

add_library (cpsapi STATIC ${CPSAPI_SOURCES} ${CPSAPI_HEADERS})
set_target_properties(cpsapi PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(NOT MSVC)
  target_compile_options(cpsapi PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-overloaded-virtual)
endif()

target_link_libraries(cpsapi ${LIBCOPASISE_LIBRARY_NAME})

# set include directory, so that binary dir will be included during build
target_include_directories(cpsapi PUBLIC
 
   $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
   $<INSTALL_INTERFACE:cpsapi> 
)

if (OPENMP_ENABLED)
	target_compile_options(cpsapi PUBLIC ${OpenMP_CXX_FLAGS})
endif (OPENMP_ENABLED)


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/git-commit.h DESTINATION include/cpsapi)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpsapiConfig.h DESTINATION include/cpsapi)

foreach (file ${CPSAPI_HEADERS})
  get_filename_component(dir ${file} DIRECTORY)
  string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR} include/cpsapi destination ${dir})
  install(FILES ${file} DESTINATION ${destination})
endforeach()

install(TARGETS cpsapi EXPORT cpsapi-config
    RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
    INCLUDES DESTINATION include
    )

install (EXPORT cpsapi-config
    DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake
    )

WRITE_BASIC_PACKAGE_VERSION_FILE(
    ${CMAKE_CURRENT_BINARY_DIR}/cpsapi-config-version.cmake
    VERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}
    COMPATIBILITY AnyNewerVersion
    )

install (FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cpsapi-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake)

