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

#include "cpsapi/core/cpsapiValue.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiValue::Properties cpsapiValue::SupportedProperties =
  {
    cpsapiProperty::Type::VALUE
  };

cpsapiValue::cpsapiValue(wrapped * pWrapped)
  : base(pWrapped, Type::Value)
{
  if (cpsapiFactory::getDataType(pWrapped) == CDataValue::Type::INVALID)
    operator=(nullptr);
}

cpsapiValue::cpsapiValue(const cpsapiValue & src)
  : base(src)
{}

cpsapiValue::~cpsapiValue()
{}

// virtual 
void cpsapiValue::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Value);
  base::accept(visitor);
}

bool cpsapiValue::setProperty(const cpsapiValue::Property & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiVariant cpsapiValue::getProperty(const cpsapiValue::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiValue::setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty<cpsapiValue>(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  switch (cpsapiFactory::getDataType(getObject()))
  {
    case CDataValue::Type::DOUBLE:
      if (value.getType() == cpsapiVariant::Type::Double)
        {
          *(double *) getObject()->getValuePointer() = value.toDouble();
          return true;
        }
      break;

    case CDataValue::Type::INT:
      if (value.getType() == cpsapiVariant::Type::Int32)
        {
          *(C_INT32 *) getObject()->getValuePointer() = value.toInt32();
          return true;
        }
      break;
      
    case CDataValue::Type::UINT:
      if (value.getType() == cpsapiVariant::Type::UnsignedInt32)
        {
          *(unsigned C_INT32 *) getObject()->getValuePointer() = value.toUnsignedInt32();
          return true;
        }
      break;
      
    case CDataValue::Type::BOOL:
      if (value.getType() == cpsapiVariant::Type::Bool)
        {
          *(bool *) getObject()->getValuePointer() = value.toBool();
          return true;
        }
      break;
      
    case CDataValue::Type::STRING:
      if (value.getType() == cpsapiVariant::Type::String)
        {
          *(std::string *) getObject()->getValuePointer() = value.toString();
          return true;
        }
      break;
      
    default:
      break;      
  }

  return false;
}

// virtual
cpsapiVariant cpsapiValue::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiVariant();

  if (!isValidProperty<cpsapiValue>(property))
    return base::getProperty(property, framework);

  switch (cpsapiFactory::getDataType(getObject()))
  {
    case CDataValue::Type::DOUBLE:
      return *(double *) getObject()->getValuePointer();
      break;

    case CDataValue::Type::INT:
      return *(C_INT32 *) getObject()->getValuePointer();
      break;
      
    case CDataValue::Type::UINT:
      return *(unsigned C_INT32 *) getObject()->getValuePointer();
      break;
      
    case CDataValue::Type::BOOL:
      return *(bool *) getObject()->getValuePointer();
      break;
      
    case CDataValue::Type::STRING:
      return *(std::string *) getObject()->getValuePointer();
      break;
      
    default:
      break;      
  }

  return cpsapiVariant();
}

cpsapiValue::operator cpsapiVariant() const
{
  return getProperty(cpsapiProperty::VALUE, CCore::Framework::__SIZE);
}

bool cpsapiValue::valid() const
{
  return cpsapiFactory::getDataType(getObject()) != CDataValue::Type::INVALID;
}
