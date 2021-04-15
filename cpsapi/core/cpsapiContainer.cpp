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

#include "cpsapi/core/cpsapiContainer.h"

#include <copasi/core/CDataContainer.h>

#include <algorithm>

CPSAPI_NAMESPACE_USE

// static
const cpsapiObject::Properties cpsapiContainer::SupportedProperties = {};

cpsapiContainer::cpsapiContainer(CDataContainer * pContainer)
  :base(pContainer)
{}

cpsapiContainer::cpsapiContainer(const cpsapiContainer & src)
  :base(src)
{}

// virtual
cpsapiContainer::~cpsapiContainer()
{}

// virtual 
void cpsapiContainer::accept(cpsapiVisitor & visitor)
{
  if (!mpObject)
    return;

  visitor.visit(*this);

  base::accept(visitor);

  CDataContainer::objectMap & Objects = static_cast< CDataContainer * >(*mpObject)->getObjects();

  std::for_each(Objects.begin(), Objects.end(), [&visitor](CDataObject * pObject) {
    if (pObject->hasFlag(CDataObject::Flag::Container))
      cpsapiContainer(static_cast< CDataContainer * >(pObject)).accept(visitor);
    else
      cpsapiObject(pObject).accept(visitor);
  });
}
