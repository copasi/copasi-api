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
#include "cpsapi/core/cpsapiValue.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"

#include <copasi/core/CDataContainer.h>
#include <copasi/core/CDataObjectReference.h>
#include <copasi/model/CModel.h>

#include <algorithm>
#include <typeindex>

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

  visitor.visit(this, cpsapiVisitor::TypeId::cpsapiContainer);

  CDataContainer::objectMap & Objects = static_cast< CDataContainer * >(*mpObject)->getObjects();

  // TODO CRITICAL We need to call the most specific cpsapiObject derived class instead of just
  // cpsapiContainer and cpsapiValue
  std::for_each(Objects.begin(), Objects.end(), [&visitor](CDataObject * pObject) {
    std::type_index ObjectIndex = std::type_index(typeid(*pObject));

    if (ObjectIndex == std::type_index(typeid(CModel)))
      return cpsapiModel(static_cast< CModel * >(pObject)).accept(visitor);

    if (ObjectIndex == std::type_index(typeid(CMetab)))
      return cpsapiSpecies(static_cast< CMetab * >(pObject)).accept(visitor);

    if (ObjectIndex == std::type_index(typeid(CCompartment)))
      return cpsapiCompartment(static_cast< CCompartment * >(pObject)).accept(visitor);

    if (ObjectIndex == std::type_index(typeid(CDataObjectReference< double >)))
      return cpsapiValue(static_cast< CDataObjectReference< double > * >(pObject)).accept(visitor);

    if (ObjectIndex == std::type_index(typeid(CDataObjectReference< int >)))
      return cpsapiValue(static_cast< CDataObjectReference< int > * >(pObject)).accept(visitor);

    if (ObjectIndex == std::type_index(typeid(CDataObjectReference< bool >)))
      return cpsapiValue(static_cast< CDataObjectReference< bool > * >(pObject)).accept(visitor);

    // Catch all for all container
    if (dynamic_cast< CDataContainer * >(pObject) != nullptr)
      return cpsapiContainer(static_cast< CDataContainer * >(pObject)).accept(visitor);
  });
}
