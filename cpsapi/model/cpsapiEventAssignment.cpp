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

#include "cpsapi/model/cpsapiEventAssignment.h"
#include "cpsapi/model/cpsapiTransaction.h"

#pragma GCC diagnostic push
#include <copasi/model/CEvent.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static
const cpsapiEventAssignment::Properties cpsapiEventAssignment::SupportedProperties =
{
  cpsapiProperty::EXPRESSION,
  cpsapiProperty::OBJECT_REFERENCE_CN
};

// static
const cpsapiEventAssignment::Properties cpsapiEventAssignment::HiddenProperties = {};

// static
const cpsapiEventAssignment::References cpsapiEventAssignment::SupportedReferences = {};

// static
const cpsapiEventAssignment::References cpsapiEventAssignment::HiddenReferences = {};

cpsapiEventAssignment::cpsapiEventAssignment(wrapped * pWrapped, const cpsapiObjectData::Type & type)
  : base(pWrapped, type)
{}

// virtual
cpsapiEventAssignment::~cpsapiEventAssignment()
{}

bool cpsapiEventAssignment::setProperty(const cpsapiEventAssignment::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast<const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiEventAssignment::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast<const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiEventAssignment::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!isValid()
      || isHiddenProperty< cpsapiEventAssignment >(property))
    return false;

  if (!isImplementedProperty< cpsapiEventAssignment >(property))
    return base::setProperty(property, value, framework);

  wrapped * pWrapped = WRAPPED;
  CDataObject * pChangedObject = pWrapped;
  bool success = false;

  switch (property)
    {
    case cpsapiProperty::EXPRESSION:
      if (value.getType() == cpsapiData::Type::String)
        success = pWrapped->setExpression(value.toString());
      break;
    
    case cpsapiProperty::OBJECT_REFERENCE_CN:
      if (value.getType() == cpsapiData::Type::CommonName )
        success = pWrapped->setTargetCN(value.toCommonName());
      else if (value.getType() == cpsapiData::Type::String)
        success = pWrapped->setTargetCN(value.toString());
      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, framework);
}

// virtual 
cpsapiData cpsapiEventAssignment::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!isValid()
      || isHiddenProperty< cpsapiEventAssignment >(property))
    return cpsapiData();

  if (!isImplementedProperty<cpsapiEventAssignment>(property))
    return base::getProperty(property, framework);

  wrapped * pWrapped = WRAPPED;
  
  switch (property)
    {
    case cpsapiProperty::EXPRESSION:
      return pWrapped->getExpression();
      break;
    
    case cpsapiProperty::OBJECT_REFERENCE_CN:
      return CCommonName(pWrapped->getTargetCN());
      break;

    default:
      break;
    }

  return cpsapiData();
}

CCommonName cpsapiEventAssignment::getDataCN(const cpsapiEventAssignment::Reference & reference, const CCore::Framework & framework) const
{
  return getDataCN(static_cast< cpsapiReference::Type >(reference), framework);
}

// virtual
CCommonName cpsapiEventAssignment::getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const
{
  if (!isValid()
      || isHiddenReference< cpsapiEventAssignment >(reference))
    return Invalid;

  if (!isImplementedReference< cpsapiEventAssignment >(reference))
    return base::getDataCN(reference, framework);

  switch (reference)
    {
    default:
      break;
    }

  return Invalid;
}
