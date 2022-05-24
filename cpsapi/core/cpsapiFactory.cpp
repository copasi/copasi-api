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
      insert< cpsapiVector< cpsapiCompartment >, CDataVectorNS< CCompartment > >(PartType::vectorCompartment);
      insert< cpsapiVector< cpsapiSpecies >, CDataVector< CMetab > >(PartType::vectorSpecies);
      insert< cpsapiVector< cpsapiSpecies >, CDataVectorNS< CMetab > >(PartType::vectorSpecies);
      insert< cpsapiVector< cpsapiGlobalQuantity >, CDataVectorN< CModelValue > >(PartType::vectorGlobalQuantity);
      insert< cpsapiVector< cpsapiReaction >, CDataVectorNS< CReaction > >(PartType::vectorReaction);
      insert< cpsapiVector< cpsapiDataModel >, CDataVector< CDataModel > >(PartType::vectorDataModel);
      insert< cpsapiVector< cpsapiTask >, CDataVectorN< CCopasiTask > >(PartType::vectorTask);
      insert< cpsapiValue, CDataObjectReference< C_FLOAT64 > >(PartType::value);
      insert< cpsapiValue, CDataObjectReference< C_INT32 > >(PartType::value);
      insert< cpsapiValue, CDataObjectReference< unsigned C_INT32 > >(PartType::value);
      insert< cpsapiValue, CDataObjectReference< size_t > >(PartType::value);
      insert< cpsapiValue, CDataObjectReference< std::string > >(PartType::value);
      insert< cpsapiValue, CDataObjectReference< CCommonName > >(PartType::value);
      insert< cpsapiModel, CModel >(PartType::model);
      insert< cpsapiCompartment, CCompartment >(PartType::compartment);
      insert< cpsapiSpecies, CMetab >(PartType::species);
      insert< cpsapiGlobalQuantity, CModelValue >(PartType::globalQuantity);
      insert< cpsapiReaction, CReaction >(PartType::reaction);
      insert< cpsapiDataModel, CDataModel >(PartType::dataModel);
      insert< cpsapiParameter, CCopasiParameter >(PartType::parameter);
      insert< cpsapiGroup, CCopasiParameterGroup >(PartType::group);
      insert< cpsapiMethod, CCopasiMethod >(PartType::method);
      insert< cpsapiProblem, CCopasiProblem >(PartType::problem);
      insert< cpsapiTask, CCopasiTask >(PartType::task);
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
