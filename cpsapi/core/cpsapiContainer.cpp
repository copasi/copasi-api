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

#include "cpsapi/core/cpsapiContainer.h"

#include <copasi/core/CDataContainer.h>

CPSAPI_NAMESPACE_BEGIN

cpsapiContainer::cpsapiContainer(CDataContainer * pContainer)
  :base(pContainer)
{
  if (dynamic_cast< CDataContainer * >(mpObject) == nullptr)
    mpObject = nullptr;
}

cpsapiContainer::cpsapiContainer(const cpsapiContainer & src)
  :base(src)
{}

// virtual
cpsapiContainer::~cpsapiContainer()
{}

// virtual 
void cpsapiContainer::accept(cpsapiVisitor & v)
{
  base::accept(v);

  CDataContainer * pContainer = static_cast< CDataContainer * >(mpObject);

  CDataContainer::objectMap & Objects = pContainer->getObjects();
  CDataContainer::objectMap::iterator it = Objects.begin();
  CDataContainer::objectMap::iterator end = Objects.end();

  for (; it != end; ++it)
    if (it->hasFlag(CDataObject::Flag::Container))
      cpsapiContainer(static_cast< CDataContainer * >(*it)).accept(v);
    else
      cpsapiObject(*it).accept(v);
}

CPSAPI_NAMESPACE_END