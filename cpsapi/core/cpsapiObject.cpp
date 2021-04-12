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

#include "cpsapi/core/cpsapiObject.h"

#include <copasi/core/CDataObject.h>

CPSAPI_NAMESPACE_USE

// static
cpsapiObject::Properties cpsapiObject::SupportedProperties = 
{
  CData::Property::OBJECT_NAME
};

cpsapiObject::cpsapiObject(CDataObject * pObject)
  : mpObject(pObject)
  , mpSupportedProperties(&SupportedProperties)
{}

cpsapiObject::cpsapiObject(const cpsapiObject & src)
  : mpObject(src.mpObject)
  , mpSupportedProperties(src.mpSupportedProperties)
{}

// virtual
cpsapiObject::~cpsapiObject()
{}

// virtual 
void cpsapiObject::accept(cpsapiVisitor& v)
{
  if (!mpObject)
    return;

  v.visit(*this);
}
  
CDataObject * cpsapiObject::getObject()
{
  return *mpObject;
}

const CDataObject * cpsapiObject::getObject() const
{
  return *mpObject;
}

bool cpsapiObject::set(const cpsapiObject::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast< CData::Property >(property), value, framework);
}

bool cpsapiObject::set(const std::string & property, const CDataValue & value, const std::string & framework)
{
  return set(CData::PropertyName.toEnum(property), value, CCore::FrameworkNames.toEnum(framework));
}

bool cpsapiObject::isValidProperty(const std::string & property) const
{
  return isValidProperty(CData::PropertyName.toEnum(property));
}

// virtual
bool cpsapiObject::isValidProperty(const CData::Property & property) const
{
  return mpSupportedProperties->find(property) != mpSupportedProperties->end();
}

// virtual
bool cpsapiObject::set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!isValidProperty(property))
    return false;

  bool success = false;

  switch (property)
    {
    case CData::Property::OBJECT_NAME:
      if (value.getType() == CDataValue::Type::STRING)
        success = mpObject->setObjectName(value.toString());

      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(*mpObject, framework);
}

cpsapiObject::operator bool() const
{
  return mpObject;
}
