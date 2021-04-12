# BEGIN: Copyright 
# Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
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

file(GLOB CURRENT_DIR_HEADERS model/*.h)
file(GLOB CURRENT_DIR_SOURCES model/*.cpp)

# create source group for IDEs
source_group(model FILES ${CURRENT_DIR_SOURCES} ${CURRENT_DIR_HEADERS})

# add files to main CPSAPI sources
set(CPSAPI_SOURCES ${CPSAPI_SOURCES} ${CURRENT_DIR_SOURCES})
set(CPSAPI_HEADERS ${CPSAPI_HEADERS} ${CURRENT_DIR_HEADERS})
