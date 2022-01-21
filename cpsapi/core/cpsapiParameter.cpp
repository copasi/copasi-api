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

#include "cpsapi/core/cpsapiParameter.h"

#pragma GCC diagnostic push
#include <copasi/utilities/CCopasiParameter.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static
const cpsapiParameter::Properties cpsapiParameter::SupportedProperties =
{
  cpsapiProperty::Type::PARAMETER_VALUE
};

// static
const cpsapiParameter::Properties cpsapiParameter::HiddenProperties = {};

// static
const cpsapiParameter::References cpsapiParameter::SupportedReferences =
{
  cpsapiProperty::Type::PARAMETER_VALUE
};

// static
const cpsapiParameter::References cpsapiParameter::HiddenReferences = {};

cpsapiParameter::cpsapiParameter(wrapped * pWrapped, const cpsapiObject::Type & type)
  : base(pWrapped, type)
{}

// virtual
cpsapiParameter::~cpsapiParameter()
{}

// virtual 
void cpsapiParameter::accept(cpsapiVisitor & visitor)
{
  if (!isValid())
    return;

  visitor.visit(this, Type::Parameter);
  base::accept(visitor);
}

bool cpsapiParameter::setProperty(const cpsapiParameter::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast<const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiParameter::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast<const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiParameter::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!isValid()
      && isHiddenProperty< cpsapiParameter >(property))
    return false;

  if (!isImplementedProperty< cpsapiParameter >(property))
    return base::setProperty(property, value, framework);

  wrapped * pParameter = WRAPPED;
  bool success = false;

  switch (property)
    {
    case cpsapiProperty::Type::PARAMETER_VALUE:
      switch (pParameter->getType())
        {
        case wrapped::Type::DOUBLE:
        case wrapped::Type::UDOUBLE:
          if (value.getType() == cpsapiData::Type::Double)
            pParameter->setValue(value.toDouble());
          break;

        case wrapped::Type::INT:
          if (value.getType() == cpsapiData::Type::Int32)
            success = pParameter->setValue(value.toInt32());
          break;

        case wrapped::Type::UINT:
          if (value.getType() == cpsapiData::Type::UnsignedInt32)
            success = pParameter->setValue(value.toUnsignedInt32());
          break;

        case wrapped::Type::BOOL:
          if (value.getType() == cpsapiData::Type::Bool)
            success = pParameter->setValue(value.toBool());
          break;

        case wrapped::Type::STRING:
        case wrapped::Type::KEY:
        case wrapped::Type::FILE:
        case wrapped::Type::EXPRESSION:
          if (value.getType() == cpsapiData::Type::String)
            success = pParameter->setValue(value.toString());
          break;

        case wrapped::Type::CN:
          if (value.getType() == cpsapiData::Type::String)
            success = pParameter->setValue(CCommonName (value.toString()));
          break;

        case wrapped::Type::GROUP:
        case wrapped::Type::INVALID:
        case wrapped::Type::__SIZE:
          break;
        }

    default:
      break;
    }

  return success;
}

// virtual 
cpsapiData cpsapiParameter::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!isValid()
      && isHiddenProperty< cpsapiParameter >(property))
    return cpsapiData();

  if (!isImplementedProperty<cpsapiParameter>(property))
    return base::getProperty(property, framework);

  wrapped * pParameter = WRAPPED;

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
        case wrapped::Type::__SIZE:
          break;
        }
      break;

    default:
      break;
    }

  return cpsapiData();
}

CCommonName cpsapiParameter::getDataCN(const cpsapiParameter::Reference & reference, const CCore::Framework & framework) const
{
  return getDataCN(static_cast< cpsapiReference::Type >(reference), framework);
}

// virtual
CCommonName cpsapiParameter::getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const
{
  if (!isValid()
      || isHiddenReference< cpsapiParameter >(reference))
    return Invalid;

  if (!isImplementedReference< cpsapiParameter >(reference))
    return base::getDataCN(reference, framework);

  switch (reference)
    {
    case cpsapiReference::Type::PARAMETER_VALUE:
      return getObject()->getCN();
      break;

    default:
      break;
    }

  return Invalid;
}
