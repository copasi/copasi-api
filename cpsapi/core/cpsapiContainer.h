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

#pragma once

#include <copasi/core/CDataContainer.h>

#include "cpsapi/core/cpsapiObject.h"

CPSAPI_NAMESPACE_BEGIN

class cpsapiContainer: public cpsapiObject
{
public:
  cpsapiContainer() = delete;

  cpsapiContainer(CDataContainer * pContainer);

  cpsapiContainer(const cpsapiObject & src);

  virtual ~cpsapiContainer();

};

CPSAPI_NAMESPACE_END