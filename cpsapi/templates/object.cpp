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

#include "cpsapi/templates/object.h"

#include <copasi/utilities/CCopasiParameter.h>

CPSAPI_NAMESPACE_USE

// static
const cpsapiTemplate::Properties cpsapiTemplate::SupportedProperties =
{
  cpsapiProperty::Type::PARAMETER_VALUE
};

// static
const cpsapiTemplate::Properties cpsapiTemplate::HiddenProperties = {};

// static
const cpsapiTemplate::References cpsapiTemplate::SupportedReferences =
{
  cpsapiProperty::Type::PARAMETER_VALUE
};

// static
const cpsapiTemplate::References cpsapiTemplate::HiddenReferences = {};

cpsapiTemplate::cpsapiTemplate(wrapped * pWrapped, const cpsapiObject::Type & type)
  : base(pWrapped, type)
{}

cpsapiTemplate::cpsapiTemplate(const cpsapiTemplate & src)
  : base(src)
{}

// virtual
cpsapiTemplate::~cpsapiTemplate()
{}

// virtual 
void cpsapiTemplate::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Parameter);
  base::accept(visitor);
}

bool cpsapiTemplate::setProperty(const cpsapiTemplate::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast<const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiTemplate::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast<const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiTemplate::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!operator bool()
      && isHiddenProperty< cpsapiTemplate >(property))
    return false;

  if (!isImplementedProperty< cpsapiTemplate >(property))
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
cpsapiData cpsapiTemplate::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool()
      && isHiddenProperty< cpsapiTemplate >(property))
    return cpsapiData();

  if (!isImplementedProperty<cpsapiTemplate>(property))
    return base::getProperty(property, framework);

  wrapped * pParameter = WRAPPED;
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

  return cpsapiData();
}

CCommonName cpsapiTemplate::getDataCN (const cpsapiTemplate::Reference & reference, const CCore::Framework & framework) const
{
  getDataCN (static_cast< cpsapiReference::Type >(reference), framework);
}

// virtual
CCommonName cpsapiTemplate::getDataCN (const cpsapiReference::Type & reference, const CCore::Framework & framework) const
{
  if (!operator bool()
      || isHiddenReference< cpsapiTemplate >(reference))
    return Invalid;

  if (!isImplementedReference< cpsapiTemplate >(reference))
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
