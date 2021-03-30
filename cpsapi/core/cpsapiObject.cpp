// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#include "cpsapi/core/cpsapiObject.h"

#include <copasi/core/CDataObject.h>

CPSAPI_NAMESPACE_BEGIN
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

CDataObject * cpsapiObject::getObject()
{
  return mpObject;
}

const CDataObject * cpsapiObject::getObject() const
{
  return mpObject;
}

bool cpsapiObject::set(const cpsapiObject::Property & property, const CDataValue & value)
{
  return set(static_cast<const CData::Property >(property), value);
}

bool cpsapiObject::set(const std::string & property, const CDataValue & value)
{
  CData::Property P = CData::PropertyName.toEnum(property);

  return set(CData::PropertyName.toEnum(property), value);
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
bool cpsapiObject::set(const CData::Property & property,
                       const CDataValue & value)
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

  return success && cpsapiTransaction::synchronize(mpObject);
}

cpsapiObject::operator bool() const
{
  return mpObject != nullptr;
}

CPSAPI_NAMESPACE_END