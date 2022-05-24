// BEGIN: Copyright 
// Copyright (C) 2021 - 2022 by Pedro Mendes, Rector and Visitors of the 
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

#include <iostream>

#include "cpsapi/cpsapiConfig.h"
#include <cpsapi/model/cpsapiModel.h>

CPSAPI_NAMESPACE_BEGIN

class Visitor
{
public:
  virtual ~Visitor() {}

  template< class Visited > 
  void visit(Visited * pObject) = delete;
};

template < >
void Visitor::visit< cpsapiObject >(cpsapiObject * pObject)
{
  std::cout << "visit cpsapiObject: " << (*pObject)->getCN() << std::endl;
}

template < >
void Visitor::visit< cpsapiModel >(cpsapiModel * pObject)
{
  std::cout << "visit cpsapiModel: " << (*pObject)->getCN() << std::endl;
}
CPSAPI_NAMESPACE_END


#include <cpsapi/core/cpsapiVisitor.hpp>
