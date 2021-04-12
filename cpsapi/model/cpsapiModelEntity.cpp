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

#include "cpsapi/model/cpsapiModelEntity.h"

#include <copasi/model/CModelValue.h>

CPSAPI_NAMESPACE_BEGIN
CPSAPI_NAMESPACE_USE

// static
cpsapiModelEntity::Properties cpsapiModelEntity::SupportedProperties =
  {
    CData::Property::EXPRESSION,
    CData::Property::INITIAL_EXPRESSION,
    CData::Property::INITIAL_VALUE,
    CData::Property::SIMULATION_TYPE,
    CData::Property::ADD_NOISE,
    CData::Property::NOISE_EXPRESSION
  };

cpsapiModelEntity::cpsapiModelEntity(CModelEntity * pObject)
  : base(pObject)
{
  mpSupportedProperties = & cpsapiModelEntity::SupportedProperties;
}

cpsapiModelEntity::cpsapiModelEntity(const cpsapiModelEntity & src)
  : base(src)
{}

// virtual
cpsapiModelEntity::~cpsapiModelEntity()
{}

bool cpsapiModelEntity::set(const cpsapiModelEntity::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast<const CData::Property >(property), value, framework);
}

CDataValue cpsapiModelEntity::get(const Property & property, const CCore::Framework & framework) const
{
  return get(static_cast<const CData::Property >(property), framework);
}

// virtual
bool cpsapiModelEntity::set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (*mpObject == nullptr)
    return false;

  if (!isValidProperty(property))
    return base::set(property, value, framework);

  CModelEntity * pEntity = static_cast< CModelEntity * >(*mpObject);
  bool success = false;

  CDataObject * pChangedObject = pEntity;

  switch (property)
    {
    case CData::Property::EXPRESSION:
      if (value.getType() == CDataValue::Type::STRING)
        success = pEntity->setExpression(value.toString());

      break;

    case CData::Property::INITIAL_EXPRESSION:
      if (value.getType() == CDataValue::Type::STRING)
        success = pEntity->setInitialExpression(value.toString());

      break;

    case CData::Property::INITIAL_VALUE:
      if (value.getType() == CDataValue::Type::DOUBLE)
        {
          pChangedObject = pEntity->getInitialValueReference();
          pEntity->setInitialValue(value.toDouble());
          success = true;
        }

      break;

    case CData::Property::SIMULATION_TYPE:
      if (value.getType() == CDataValue::Type::STRING)
        success = pEntity->setStatus(CModelEntity::StatusName.toEnum(value.toString()));

      break;

    case CData::Property::ADD_NOISE:
      if (value.getType() == CDataValue::Type::BOOL)
        {
          pEntity->setHasNoise(value.toBool());
          success = true;
        }

      break;

    case CData::Property::NOISE_EXPRESSION:
      if (value.getType() == CDataValue::Type::STRING)
        success = pEntity->setNoiseExpression(value.toString());

      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, framework);
}

// virtual 
CDataValue cpsapiModelEntity::get(const CData::Property & property, const CCore::Framework & framework) const
{
  if (*mpObject == nullptr)
    return CDataValue();

  if (!isValidProperty(property))
    return base::get(property, framework);

  CModelEntity * pEntity = static_cast< CModelEntity * >(*mpObject);
  bool success = false;

  CDataObject * pChangedObject = pEntity;

  switch (property)
    {
    case CData::Property::EXPRESSION:
      return pEntity->getExpression();
      break;

    case CData::Property::INITIAL_EXPRESSION:
      return pEntity->getInitialExpression();
      break;

    case CData::Property::INITIAL_VALUE:
      return  pEntity->getInitialValue();
      break;

    case CData::Property::SIMULATION_TYPE:
      return CModelEntity::StatusName[pEntity->getStatus()];

      break;

    case CData::Property::ADD_NOISE:
      return pEntity->hasNoise();
      break;

    case CData::Property::NOISE_EXPRESSION:
      return pEntity->getNoiseExpression();

      break;

    default:
      break;
    }

  return CDataValue();
}

CPSAPI_NAMESPACE_END