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

cpsapiParameter::cpsapiParameter(wrapped * pWrapped, const cpsapiObject::Type & type)
  : base(pWrapped, type)
{}

cpsapiParameter::cpsapiParameter(const cpsapiParameter & src)
  : base(src)
{}

// virtual
cpsapiParameter::~cpsapiParameter()
{}

// virtual 
void cpsapiParameter::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Parameter);
  base::accept(visitor);
}

bool cpsapiParameter::setProperty(const cpsapiParameter::Property & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  return setProperty(static_cast<const cpsapiProperty::Type >(property), value, framework);
}

cpsapiVariant cpsapiParameter::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast<const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiParameter::setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty<cpsapiParameter>(property))
    return base::setProperty(property, value, framework);

  wrapped * pParameter = static_cast< wrapped * >(getObject());
  bool success = false;

  switch (property)
    {
    case cpsapiProperty::Type::PARAMETER_VALUE:
      switch (pParameter->getType())
        {
        case wrapped::Type::DOUBLE:
        case wrapped::Type::UDOUBLE:
          if (value.getType() == cpsapiVariant::Type::Double)
            pParameter->setValue(value.toDouble());
          break;

        case wrapped::Type::INT:
          if (value.getType() == cpsapiVariant::Type::Int32)
            success = pParameter->setValue(value.toInt32());
          break;

        case wrapped::Type::UINT:
          if (value.getType() == cpsapiVariant::Type::UnsignedInt32)
            success = pParameter->setValue(value.toUnsignedInt32());
          break;

        case wrapped::Type::BOOL:
          if (value.getType() == cpsapiVariant::Type::Bool)
            success = pParameter->setValue(value.toBool());
          break;

        case wrapped::Type::STRING:
        case wrapped::Type::KEY:
        case wrapped::Type::FILE:
        case wrapped::Type::EXPRESSION:
          if (value.getType() == cpsapiVariant::Type::String)
            success = pParameter->setValue(value.toString());
          break;

        case wrapped::Type::CN:
          if (value.getType() == cpsapiVariant::Type::String)
            success = pParameter->setValue(CCommonName(value.toString()));
          break;

        case wrapped::Type::GROUP:
        case wrapped::Type::INVALID:
          break;
        }

    default:
      break;
    }

  return success;
}

// virtual 
cpsapiVariant cpsapiParameter::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiVariant();

  if (!isValidProperty<cpsapiParameter>(property))
    return base::getProperty(property, framework);

  wrapped * pParameter = static_cast< wrapped * >(getObject());
  bool success = false;

  switch (property)
    {
    case cpsapiProperty::Type::PARAMETER_VALUE:
      switch (pParameter->getType())
        {
        case wrapped::Type::DOUBLE:
        case wrapped::Type::UDOUBLE:
          return * (C_FLOAT64 *) pParameter->getValidValuesPointer();
          break;

        case wrapped::Type::INT:
          return * (C_INT32 *) pParameter->getValidValuesPointer();
          break;

        case wrapped::Type::UINT:
          return * (unsigned C_INT32 *) pParameter->getValidValuesPointer();
          break;

        case wrapped::Type::BOOL:
          return * (bool *) pParameter->getValidValuesPointer();
          break;

        case wrapped::Type::STRING:
        case wrapped::Type::KEY:
        case wrapped::Type::FILE:
        case wrapped::Type::EXPRESSION:
        case wrapped::Type::CN:
          return * (std::string *) pParameter->getValidValuesPointer();
          break;

        case wrapped::Type::GROUP:
        case wrapped::Type::INVALID:
          break;
        }
      break;

    default:
      break;
    }

  return cpsapiVariant();
}
