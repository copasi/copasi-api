// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#pragma once

#include "cpsapi/core/cpsapiObject.h"

class CDataContainer;

CPSAPI_NAMESPACE_BEGIN

class cpsapiContainer : public cpsapiObject
{
private:
  typedef cpsapiObject base;

public:
  cpsapiContainer() = delete;

  cpsapiContainer(CDataContainer * pContainer);

  cpsapiContainer(const cpsapiContainer & src);

  virtual ~cpsapiContainer();

  virtual void accept(cpsapiVisitor & v) override;
};

CPSAPI_NAMESPACE_END