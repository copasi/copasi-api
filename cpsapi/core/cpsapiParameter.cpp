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

#include "cpsapi/core/cpsapiParameter.h"

#include <copasi/utilities/CCopasiParameter.h>

CPSAPI_NAMESPACE_USE

// static
const cpsapiParameter::Properties cpsapiParameter::SupportedProperties =
  {
    cpsapiProperty::Type::PARAMETER_VALUE
  };

cpsapiParameter::cpsapiParameter(CCopasiParameter * pObject, const cpsapiObject::Type & typeId)
  : base(pObject, typeId)
  , mpDefaultParameter()
{}

cpsapiParameter::cpsapiParameter(const cpsapiParameter & src)
  : base(src)
  , mpDefaultParameter(src.mpDefaultParameter)
{}

// virtual
cpsapiParameter::~cpsapiParameter()
{}

// virtual 
void cpsapiParameter::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::cpsapiParameter);
  base::accept(visitor);
}

bool cpsapiParameter::setProperty(const cpsapiParameter::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return setProperty(static_cast<const cpsapiProperty::Type >(property), value, framework);
}

CDataValue cpsapiParameter::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast<const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiParameter::setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty<cpsapiParameter>(property))
    return base::setProperty(property, value, framework);

  CCopasiParameter * pParameter = static_cast< CCopasiParameter * >(getObject());
  bool success = false;

  switch (property)
    {
    case cpsapiProperty::Type::PARAMETER_VALUE:
      switch (pParameter->getType())
        {
        case CCopasiParameter::Type::DOUBLE:
        case CCopasiParameter::Type::UDOUBLE:
          if (value.getType() == CDataValue::Type::DOUBLE)
            pParameter->setValue(value.toDouble());
          break;

        case CCopasiParameter::Type::INT:
          if (value.getType() == CDataValue::Type::INT)
            success = pParameter->setValue(value.toUint());
          break;

        case CCopasiParameter::Type::UINT:
          if (value.getType() == CDataValue::Type::UINT)
            success = pParameter->setValue(value.toInt());
          break;

        case CCopasiParameter::Type::BOOL:
          if (value.getType() == CDataValue::Type::BOOL)
            success = pParameter->setValue(value.toBool());
          break;

        case CCopasiParameter::Type::STRING:
        case CCopasiParameter::Type::KEY:
        case CCopasiParameter::Type::FILE:
        case CCopasiParameter::Type::EXPRESSION:
          if (value.getType() == CDataValue::Type::STRING)
            success = pParameter->setValue(value.toString());
          break;

        case CCopasiParameter::Type::CN:
          if (value.getType() == CDataValue::Type::STRING)
            success = pParameter->setValue(CCommonName(value.toString()));
          break;

        case CCopasiParameter::Type::GROUP:
        case CCopasiParameter::Type::INVALID:
          break;
        }

    default:
      break;
    }

  return success;
}

// virtual 
CDataValue cpsapiParameter::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return CDataValue();

  if (!isValidProperty<cpsapiParameter>(property))
    return base::getProperty(property, framework);

  CCopasiParameter * pParameter = static_cast< CCopasiParameter * >(getObject());
  bool success = false;

  switch (property)
    {
    case cpsapiProperty::Type::PARAMETER_VALUE:
      switch (pParameter->getType())
        {
        case CCopasiParameter::Type::DOUBLE:
        case CCopasiParameter::Type::UDOUBLE:
          return * (C_FLOAT64 *) pParameter->getValidValuesPointer();
          break;

        case CCopasiParameter::Type::INT:
          return * (C_INT32 *) pParameter->getValidValuesPointer();
          break;

        case CCopasiParameter::Type::UINT:
          return * (unsigned C_INT32 *) pParameter->getValidValuesPointer();
          break;

        case CCopasiParameter::Type::BOOL:
          return * (bool *) pParameter->getValidValuesPointer();
          break;

        case CCopasiParameter::Type::STRING:
        case CCopasiParameter::Type::KEY:
        case CCopasiParameter::Type::FILE:
        case CCopasiParameter::Type::EXPRESSION:
        case CCopasiParameter::Type::CN:
          return * (std::string *) pParameter->getValidValuesPointer();
          break;

        case CCopasiParameter::Type::GROUP:
        case CCopasiParameter::Type::INVALID:
          break;
        }
      break;

    default:
      break;
    }

  return CDataValue();
}
