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
const cpsapiObject::Properties cpsapiObject::SupportedProperties = 
{
  cpsapiProperty::Type::OBJECT_NAME
};

// static
const cpsapiObject::Properties cpsapiObject::HiddenProperties = 
{};

cpsapiObject::cpsapiObject(CDataObject * pObject)
  : cpsapiPointer(pObject)
{}

cpsapiObject::cpsapiObject(const cpsapiObject & src)
  : cpsapiPointer(src)
{}

// virtual
cpsapiObject::~cpsapiObject()
{}

CDataObject * cpsapiObject::getObject()
{
  return cpsapiPointer::operator*();
}

CDataObject * cpsapiObject::getObject() const
{
  return cpsapiPointer::operator*();
}

bool cpsapiObject::set(const cpsapiObject::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast< cpsapiProperty::Type >(property), value, framework);
}

bool cpsapiObject::set(const std::string & property, const CDataValue & value, const std::string & framework)
{
  return set(cpsapiProperty::Name.toEnum(property), value, CCore::FrameworkNames.toEnum(framework));
}

CDataValue cpsapiObject::get(const Property & property, const CCore::Framework & framework ) const
{
  return get(static_cast< cpsapiProperty::Type >(property), framework);
}

CDataValue cpsapiObject::get(const std::string & property, const std::string & framework) const
{
  return get(cpsapiProperty::Name.toEnum(property), CCore::FrameworkNames.toEnum(framework));
}

std::string cpsapiObject::displayName() const
{
  if (operator bool())
    return getObject()->getObjectDisplayName();

  return "@@@";
}

// virtual
bool cpsapiObject::set(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!isValidProperty<cpsapiObject>(property))
    return false;

  bool success = false;

  switch (property)
    {
    case cpsapiProperty::Type::OBJECT_NAME:
      if (value.getType() == CDataValue::Type::STRING)
        success = getObject()->setObjectName(value.toString());

      break;

    default:
      break;
    }

  return success;
}

// virtual 
CDataValue cpsapiObject::get(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!isValidProperty<cpsapiObject>(property))
    return CDataValue();

  switch (property)
    {
    case cpsapiProperty::Type::OBJECT_NAME:
      return CDataValue(getObject()->getObjectName());
      break;

    default:
      break;
    }

  return CDataValue();
}

cpsapiObject::operator bool() const
{
  return cpsapiPointer::operator bool();
}
