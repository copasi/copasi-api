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

file(GLOB CORE_HEADERS core/*.h)
file(GLOB CORE_SOURCES core/*.cpp)

# create source group for IDEs
source_group(core FILES ${CORE_SOURCES} ${CORE_HEADERS})

# add files to main CPSAPI sources
set(CPSAPI_SOURCES ${CPSAPI_SOURCES} ${CORE_SOURCES})
set(CPSAPI_HEADERS ${CPSAPI_HEADERS} ${CORE_HEADERS})
