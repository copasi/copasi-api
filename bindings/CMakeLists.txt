# BEGIN: Copyright 
# Copyright (C) 2022 by Pedro Mendes, Rector and Visitors of the 
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

if (EXISTS "$ENV{SWIG_DIR}")
  SET (SWIG_DIR "$ENV{SWIG_DIR}")
endif ()

if (SWIG_DIR)
  SET (SWIG_EXECUTABLE "${SWIG_DIR}/bin/swig")
endif()

find_package(SWIG REQUIRED)

if (ENABLE_CSHARP)
  add_subdirectory(csharp)
endif (ENABLE_CSHARP)

if (ENABLE_JAVA)
  add_subdirectory(java)
endif (ENABLE_JAVA)

if (ENABLE_PYTHON)
  add_subdirectory(python)
endif (ENABLE_PYTHON)

if (ENABLE_R)
  add_subdirectory(R)
endif (ENABLE_R)


