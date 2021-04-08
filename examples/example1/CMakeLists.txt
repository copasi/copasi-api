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

file(GLOB CURRENT_DIR_HEADERS *.h)
file(GLOB CURRENT_DIR_SOURCES *.cpp)

# create source group for IDEs
source_group(example1 FILES ${CURRENT_DIR_SOURCES} ${CURRENT_DIR_HEADERS})

add_executable(example1 ${CURRENT_DIR_SOURCES})

target_link_libraries(example1 cpsapi ${LIBCOPASISE_LIBRARY})

