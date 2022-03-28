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

#include "cpsapi/core/cpsapiValue.h"
#include "cpsapi/core/cpsapiContainer.h"
#include "cpsapi/core/cpsapiVector.h"
#include "cpsapi/core/cpsapiDataModel.h"
#include "cpsapi/core/cpsapiParameter.h"
#include "cpsapi/core/cpsapiGroup.h"
#include "cpsapi/model/cpsapiModelEntity.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/task/cpsapiMethod.h"
#include "cpsapi/task/cpsapiProblem.h"
#include "cpsapi/task/cpsapiTask.h"

CPSAPI_NAMESPACE_USE

// static
cpsapiFactory::CopasiMap cpsapiFactory::copasiMap;

// static
void cpsapiFactory::init()
{
  if (copasiMap.empty())
    {
      insert< cpsapiVector< cpsapiCompartment >, CDataVectorNS< CCompartment > >();
      insert< cpsapiVector< cpsapiSpecies >, CDataVector< CMetab > >();
      insert< cpsapiVector< cpsapiSpecies >, CDataVectorNS< CMetab > >();
      insert< cpsapiVector< cpsapiGlobalQuantity >, CDataVectorN< CModelValue > >();
      insert< cpsapiVector< cpsapiReaction >, CDataVectorNS< CReaction > >();
      insert< cpsapiVector< cpsapiDataModel >, CDataVector< CDataModel > >();
      insert< cpsapiVector< cpsapiTask >, CDataVectorN< CCopasiTask > >();
      insert< cpsapiValue, CDataObjectReference< C_FLOAT64 > >();
      insert< cpsapiValue, CDataObjectReference< C_INT32 > >();
      insert< cpsapiValue, CDataObjectReference< unsigned C_INT32 > >();
      insert< cpsapiValue, CDataObjectReference< size_t > >();
      insert< cpsapiValue, CDataObjectReference< std::string > >();
      insert< cpsapiValue, CDataObjectReference< CCommonName > >();
      insert< cpsapiModel, CModel >();
      insert< cpsapiCompartment, CCompartment >();
      insert< cpsapiSpecies, CMetab >();
      insert< cpsapiGlobalQuantity, CModelValue >();
      insert< cpsapiReaction, CReaction >();
      insert< cpsapiDataModel, CDataModel >();
      insert< cpsapiParameter, CCopasiParameter >();
      insert< cpsapiGroup, CCopasiParameterGroup >();
      insert< cpsapiMethod, CCopasiMethod >();
      insert< cpsapiProblem, CCopasiProblem >();
      insert< cpsapiTask, CCopasiTask >();
    }
}

// static
const cpsapiFactory::PartInterface & cpsapiFactory::info(const std::type_index & typeIndex)
{
  static const Part< void, void > Unknown;

  CopasiMap::const_iterator found = copasiMap.find(typeIndex);

  if (found != copasiMap.end())
    return *found->second;

  return Unknown;
}

// static
const cpsapiFactory::PartInterface & cpsapiFactory::info(CDataObject * pFrom)
{
  static const Part< void, void > Unknown;

  if (pFrom != nullptr)
    return info(std::type_index(typeid(*pFrom)));

  return Unknown;
}

// static
const cpsapiFactory::PartInterface & cpsapiFactory::info(const cpsapiObject & from)
{
  return info(std::type_index(typeid(from)));
}

// static 
cpsapiObject * cpsapiFactory::copy(const cpsapiObject & object)
{
  return info(object).copy(object);
}

// static 
cpsapiObject * cpsapiFactory::create(CDataObject * pFrom)
{
  return info(pFrom).create(pFrom);
}

// static
cpsapiObject * cpsapiFactory::make(CDataObject * pDataObject, const PartInterface * pTypeInfo)
{
  const PartInterface * pInfo = pTypeInfo;

  if (pTypeInfo == nullptr)
    pInfo = &info(pDataObject);

  return pInfo->create(pDataObject);
}

// static 
CDataValue::Type cpsapiFactory::getDataType(const CObjectInterface * pObject)
{
  if (pObject == nullptr)
    return CDataValue::Type::INVALID;

  const CDataObject * pDataObject = pObject->getDataObject();

  if (pDataObject != pObject)
    return CDataValue::DOUBLE;

  if (pDataObject->hasFlag(CDataObject::ValueDbl))
    return CDataValue::Type::DOUBLE;

  if (pDataObject->hasFlag(CDataObject::ValueInt))
    return CDataValue::Type::INT;

  if (pDataObject->hasFlag(CDataObject::ValueBool))
    return CDataValue::Type::BOOL;

  if (pDataObject->hasFlag(CDataObject::ValueString))
    return CDataValue::Type::STRING;

  return CDataValue::Type::INVALID;
}
