// BEGIN: Copyright 
// Copyright (C) 2022 by Pedro Mendes, Rector and Visitors of the 
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

#include "cpsapi/core/cpsapiObjectData.h"

#pragma GCC diagnostic push
#include <copasi/core/CDataContainer.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static 
cpsapiObjectData::Map cpsapiObjectData::Manager;

// static
void cpsapiObjectData::release()
{
  Map::iterator it = Manager.begin();
  Map::iterator end = Manager.end();

  for (; it != end; ++it)
    it->second.reset();
}

// static 
void cpsapiObjectData::erase(const CDataObject * pObject)
{
  if (pObject == nullptr)
    return;

  Map::iterator found = Manager.find(const_cast< CDataObject * >(pObject));
  
  if (found == Manager.end())
    return;

  
  if (found->second.get() != nullptr)
    std::static_pointer_cast< cpsapiObjectData >(found->second)->mpObject = nullptr;

  Manager.erase(pObject);
}

// static
void cpsapiObjectData::deleted(const CDataObject * pObject)
{
  if (dynamic_cast< const CDataContainer * >(pObject))
    for (const CDataObject * pChildObject : static_cast< const CDataContainer * >(pObject)->getObjects())
      deleted(pChildObject);

  erase(pObject);
}

// static
const CEnumAnnotation< std::string, cpsapiObjectData::Type > cpsapiObjectData::TypeName(
{
  "Object",
  "Value",
  "Container",
  "Vector",
  "ModelEntity",
  "Model",
  "Compartment",
  "Species",
  "GlobalQuantity",
  "Reaction",
  "ReactionParameter",
  "Event",
  "EventAssignment",
  "DataModel",
  "Parameter",
  "Group",
  "Method",
  "Problem",
  "Task"
});

cpsapiObjectData::cpsapiObjectData(CDataObject * pObject, const Type & type)
  : mpObject(pObject)
  , mType(type)
{}

