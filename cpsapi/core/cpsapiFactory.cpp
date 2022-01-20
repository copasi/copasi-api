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

#pragma GCC diagnostic push
#include <copasi/CopasiDataModel/CDataModel.h>
#include <copasi/core/CDataObjectReference.h>
#include <copasi/utilities/CCopasiMethod.h>
#include <copasi/utilities/CCopasiProblem.h>
#include <copasi/utilities/CCopasiTask.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static
cpsapiFactory::CopasiMap cpsapiFactory::copasiMap;

// static
void cpsapiFactory::insert(const TypeInfo & typeInfo)
{
  copasiMap.insert(std::make_pair(typeInfo.copasiClass, typeInfo));
  copasiMap.insert(std::make_pair(typeInfo.cpsapiClass, typeInfo));
}

// static
void cpsapiFactory::init()
{
  if (copasiMap.empty())
    {
      TypeInfo::initProtected< cpsapiObject, CDataObject >();
      TypeInfo::initProtected< cpsapiContainer, CDataContainer >();
      TypeInfo::initProtected< cpsapiModelEntity, CModelEntity >();
      TypeInfo::init< cpsapiVector< cpsapiCompartment >, CDataVectorNS< CCompartment > >();
      TypeInfo::init< cpsapiVector< cpsapiSpecies >, CDataVector< CMetab > >();
      TypeInfo::init< cpsapiVector< cpsapiSpecies >, CDataVectorNS< CMetab > >();
      TypeInfo::init< cpsapiVector< cpsapiGlobalQuantity >, CDataVectorN< CModelValue > >();
      TypeInfo::init< cpsapiVector< cpsapiReaction >, CDataVectorNS< CReaction > >();
      TypeInfo::init< cpsapiVector< cpsapiDataModel >, CDataVector< CDataModel > >();
      TypeInfo::init< cpsapiVector< cpsapiTask >, CDataVectorN< CCopasiTask > >();
      TypeInfo::init< cpsapiValue, CDataObjectReference< C_FLOAT64 > >();
      TypeInfo::init< cpsapiValue, CDataObjectReference< C_INT32 > >();
      TypeInfo::init< cpsapiValue, CDataObjectReference< unsigned C_INT32 > >();
      TypeInfo::init< cpsapiValue, CDataObjectReference< size_t > >();
      TypeInfo::init< cpsapiValue, CDataObjectReference< std::string > >();
      TypeInfo::init< cpsapiValue, CDataObjectReference< CCommonName > >();
      TypeInfo::init< cpsapiModel, CModel >();
      TypeInfo::init< cpsapiCompartment, CCompartment >();
      TypeInfo::init< cpsapiSpecies, CMetab >();
      TypeInfo::init< cpsapiGlobalQuantity, CModelValue >();
      TypeInfo::init< cpsapiReaction, CReaction >();
      TypeInfo::init< cpsapiDataModel, CDataModel >();
      TypeInfo::init< cpsapiParameter, CCopasiParameter >();
      TypeInfo::init< cpsapiGroup, CCopasiParameterGroup >();
      TypeInfo::init< cpsapiMethod, CCopasiMethod >();
      TypeInfo::init< cpsapiProblem, CCopasiProblem >();
      TypeInfo::init< cpsapiTask, CCopasiTask >();
    }
}

cpsapiFactory::TypeInfo::TypeInfo(const std::type_index & cpsapiClass,
                                  std::shared_ptr< cpsapiFactory::createInterface > cpsapiCreate,
                                  std::shared_ptr< cpsapiFactory::copyInterface > cpsapiCopy,
                                  const std::type_index & copasiClass,
                                  const std::string copasiString)
  : cpsapiClass(cpsapiClass)
  , cpsapiCreate()
  , cpsapiCopy()
  , copasiClass(copasiClass)
  , copasiString(copasiString)
{
  this->cpsapiCreate = cpsapiCreate;
  this->cpsapiCopy = cpsapiCopy;
}

// static
const cpsapiFactory::TypeInfo & cpsapiFactory::info(const std::type_index & typeIndex)
{
  static const TypeInfo Unknown;

  CopasiMap::const_iterator found = copasiMap.find(typeIndex);

  if (found != copasiMap.end())
    return found->second;

  return Unknown;
}

// static
const cpsapiFactory::TypeInfo & cpsapiFactory::info(CDataObject * pFrom)
{
  static const TypeInfo Unknown;

  if (pFrom != nullptr)
    return info(std::type_index(typeid(*pFrom)));

  return Unknown;
}

// static
const cpsapiFactory::TypeInfo & cpsapiFactory::info(const cpsapiObject & from)
{
  return info(std::type_index(typeid(from)));
}

// static 
cpsapiObject * cpsapiFactory::copy(const cpsapiObject & object)
{
  const TypeInfo & Info = info(object);

  if (Info.cpsapiCopy)
    return (*Info.cpsapiCopy)(object);

  return nullptr;
}

// static 
cpsapiObject * cpsapiFactory::create(CDataObject * pFrom)
{
  const TypeInfo & Info = info(pFrom);

  if (Info.cpsapiCreate)
    return (*Info.cpsapiCreate)(pFrom);

  return nullptr;
}

// static
cpsapiObject * cpsapiFactory::make(CDataObject * pDataObject, const TypeInfo * pTypeInfo)
{
  cpsapiObject * pObject = nullptr;

  const TypeInfo * pInfo = pTypeInfo;

  if (pTypeInfo == nullptr)
    pInfo = &info(pDataObject);

  if (pInfo->cpsapiCreate != nullptr)
    pObject = (*pInfo->cpsapiCreate)(pDataObject);

  return pObject;
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
