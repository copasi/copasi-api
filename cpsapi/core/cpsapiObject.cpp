// BEGIN: Copyright 
// Copyright (C) 2021 Rector and Visitors of the University of Virginia 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#include <cassert>

#include "cpsapi/core/cpsapiObject.h"

CPSAPI_NAMESPACE_BEGIN
CPSAPI_NAMESPACE_USE

// static
cpsapiObject::ValidProperties cpsapiObject::__ValidProperties = {CData::Property::OBJECT_NAME};

cpsapiObject::cpsapiObject(CDataObject * pObject)
  : mpObject(pObject)
{
  assert(mpObject != NULL);
}

cpsapiObject::cpsapiObject(const cpsapiObject & src)
  : mpObject(src.mpObject)
{}

// virtual
cpsapiObject::~cpsapiObject()
{}

CDataObject & cpsapiObject::getObject()
{
  return *mpObject;
}

const CDataObject & cpsapiObject::getObject() const
{
  return *mpObject;
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

bool cpsapiObject::isValidAttribute(const std::string & property) const
{
  return isValidAttribute(CData::PropertyName.toEnum(property));
}

// virtual
bool cpsapiObject::isValidAttribute(const CData::Property & property) const
{
  return __ValidProperties.find(property) != __ValidProperties.end();
}

// virtual
bool cpsapiObject::set(const CData::Property & property,
                       const CDataValue & value)
{
  if (!isValidAttribute(property))
    return false;

  switch (property)
    {
    case CData::Property::OBJECT_NAME:
      if (value.getType() == CDataValue::Type::STRING)
        return mpObject->setObjectName(value.toString());

      return false;
      break;

    default:
      break;
    }

  return false;
}

CPSAPI_NAMESPACE_END