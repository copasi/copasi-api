// BEGIN: Copyright 
// Copyright (C) 2021 Rector and Visitors of the University of Virginia 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#include "cpsapi/core/cpsapiContainer.h"

CPSAPI_NAMESPACE_BEGIN

cpsapiContainer::cpsapiContainer(CDataContainer * pContainer)
  :cpsapiObject(pContainer)
{}

cpsapiContainer::cpsapiContainer(const cpsapiObject & src)
  :cpsapiObject(src)
{}

// virtual
cpsapiContainer::~cpsapiContainer()
{}

CPSAPI_NAMESPACE_END