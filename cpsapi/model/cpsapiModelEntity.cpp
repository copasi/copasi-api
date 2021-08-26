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
#include "cpsapi/model/cpsapiTransaction.h"

#include <copasi/model/CModelValue.h>

CPSAPI_NAMESPACE_USE

// static
const cpsapiModelEntity::Properties cpsapiModelEntity::SupportedProperties =
  {
    cpsapiProperty::Type::EXPRESSION,
    cpsapiProperty::Type::INITIAL_EXPRESSION,
    cpsapiProperty::Type::INITIAL_VALUE,
    cpsapiProperty::Type::SIMULATION_TYPE,
    cpsapiProperty::Type::ADD_NOISE,
    cpsapiProperty::Type::NOISE_EXPRESSION
  };

cpsapiModelEntity::cpsapiModelEntity(CModelEntity * pObject, const cpsapiObject::Type & type)
  : base(pObject, type)
{}

cpsapiModelEntity::cpsapiModelEntity(const cpsapiModelEntity & src)
  : base(src)
{}

// virtual
cpsapiModelEntity::~cpsapiModelEntity()
{}

// virtual 
void cpsapiModelEntity::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::cpsapiModelEntity);
  base::accept(visitor);
}

bool cpsapiModelEntity::setProperty(const cpsapiModelEntity::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return setProperty(static_cast<const cpsapiProperty::Type >(property), value, framework);
}

CDataValue cpsapiModelEntity::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast<const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiModelEntity::setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty<cpsapiModelEntity>(property))
    return base::setProperty(property, value, framework);

  CModelEntity * pEntity = static_cast< CModelEntity * >(getObject());
  CDataObject * pChangedObject = pEntity;
  bool success = cpsapiTransaction::endStructureChange(pEntity->getModel());

  switch (property)
    {
    case cpsapiProperty::Type::EXPRESSION:
      if (value.getType() == CDataValue::Type::STRING)
        success = pEntity->setExpression(value.toString());

      break;

    case cpsapiProperty::Type::INITIAL_EXPRESSION:
      if (value.getType() == CDataValue::Type::STRING)
        success = pEntity->setInitialExpression(value.toString());

      break;

    case cpsapiProperty::Type::INITIAL_VALUE:
      if (value.getType() == CDataValue::Type::DOUBLE)
        {
          pChangedObject = pEntity->getInitialValueReference();
          pEntity->setInitialValue(value.toDouble());
          success = true;
        }

      break;

    case cpsapiProperty::Type::SIMULATION_TYPE:
      if (value.getType() == CDataValue::Type::STRING)
        success = pEntity->setStatus(CModelEntity::StatusName.toEnum(value.toString()));

      break;

    case cpsapiProperty::Type::ADD_NOISE:
      if (value.getType() == CDataValue::Type::BOOL)
        {
          pEntity->setHasNoise(value.toBool());
          success = true;
        }

      break;

    case cpsapiProperty::Type::NOISE_EXPRESSION:
      if (value.getType() == CDataValue::Type::STRING)
        success = pEntity->setNoiseExpression(value.toString());

      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, framework);
}

// virtual 
CDataValue cpsapiModelEntity::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return CDataValue();

  if (!isValidProperty<cpsapiModelEntity>(property))
    return base::getProperty(property, framework);

  CModelEntity * pEntity = static_cast< CModelEntity * >(getObject());
  bool success = false;

  CDataObject * pChangedObject = pEntity;

  switch (property)
    {
    case cpsapiProperty::Type::EXPRESSION:
      return pEntity->getExpression();
      break;

    case cpsapiProperty::Type::INITIAL_EXPRESSION:
      return pEntity->getInitialExpression();
      break;

    case cpsapiProperty::Type::INITIAL_VALUE:
      return  pEntity->getInitialValue();
      break;

    case cpsapiProperty::Type::SIMULATION_TYPE:
      return CModelEntity::StatusName[pEntity->getStatus()];

      break;

    case cpsapiProperty::Type::ADD_NOISE:
      return pEntity->hasNoise();
      break;

    case cpsapiProperty::Type::NOISE_EXPRESSION:
      return pEntity->getNoiseExpression();

      break;

    default:
      break;
    }

  return CDataValue();
}
