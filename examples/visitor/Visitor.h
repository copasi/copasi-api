// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Artistic License 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   https://opensource.org/licenses/Artistic-2.0 
// END: License 

#pragma once

#include <cpsapi/core/cpsapiVisitor.h>
#include <cpsapi/core/cpsapiContainer.h>
#include <cpsapi/core/cpsapiDataModel.h>
#include <cpsapi/core/cpsapiValue.h>
#include <cpsapi/model/cpsapiModel.h>

#include <iostream>

CPSAPI_NAMESPACE_BEGIN

class Visitor : public cpsapiVisitor 
{
public:
  virtual ~Visitor() {}

  virtual void visit(cpsapiObject * pObject, const cpsapiVisitor::TypeId & typeId);
};

CPSAPI_NAMESPACE_END
