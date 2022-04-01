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

file(GLOB CURRENT_DIR_HEADERS *.h)
file(GLOB CURRENT_DIR_SOURCES *.cpp)

# create source group for IDEs
source_group(visitor FILES ${CURRENT_DIR_SOURCES} ${CURRENT_DIR_HEADERS})

add_executable(visitor ${CURRENT_DIR_SOURCES})

if (MSVC)
    target_compile_options(visitor PRIVATE -wd4068)
endif(MSVC)

set_target_properties(visitor PROPERTIES
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ${CMAKE_CXX_STANDARD_REQUIRED}
    CXX_EXTENSIONS {CMAKE_CXX_EXTENSIONS}
)
target_link_libraries(visitor cpsapi ${LIBCOPASISE_LIBRARY})

