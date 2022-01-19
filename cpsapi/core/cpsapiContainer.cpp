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

#include "cpsapi/core/cpsapiContainer.h"

#include <copasi/core/CDataContainer.h>
#include <copasi/core/CDataObjectReference.h>
#include <copasi/model/CModel.h>

#include <algorithm>
#include <typeindex>

CPSAPI_NAMESPACE_USE

cpsapiContainer::cpsapiContainer(wrapped * pWrapped, const cpsapiObject::Type & type)
  : base(pWrapped, type)
{}

cpsapiContainer::cpsapiContainer(const cpsapiContainer & src)
  : base(src)
{}

// virtual
cpsapiContainer::~cpsapiContainer()
{}

// virtual 
void cpsapiContainer::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Container);

  wrapped::objectMap & Objects = WRAPPED->getObjects();

  for (CDataObject * pDataObject : Objects)
    {
      std::shared_ptr< cpsapiObject > pObject = cpsapiFactory::make_shared< cpsapiObject >(pDataObject);

      if (pObject)
        pObject->accept(visitor);
    }

  base::accept(visitor);
}
