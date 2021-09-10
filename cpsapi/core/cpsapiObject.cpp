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
cpsapiObject::Map cpsapiObject::Manager;

// static 
void cpsapiObject::deleted(const CDataObject * pObject)
{
  if (pObject == nullptr)
    return;

  Map::iterator found = Manager.find(const_cast< CDataObject * >(pObject));
  
  if (found == Manager.end())
    return;
  
  std::static_pointer_cast< Data >(found->second)->mpObject = nullptr;

  Manager.erase(pObject);
}

// static
const CEnumAnnotation< std::string, cpsapiObject::Type > cpsapiObject::TypeName(
{
  "cpsapiObject",
  "cpsapiContainer",
  "cpsapiVector",
  "cpsapiModelEntity",
  "cpsapiValue",
  "cpsapiModel",
  "cpsapiCompartment",
  "cpsapiSpecies",
  "cpsapiGlobalQuantity",
  "cpsapiDataModel",
  "cpsapiParameter",
  "cpsapiGroup"
});

// static
const cpsapiObject::Properties cpsapiObject::SupportedProperties =
{
  cpsapiProperty::Type::OBJECT_NAME,
  cpsapiProperty::Type::DISPLAY_NAME,
  cpsapiProperty::Type::CN
};

// static
const cpsapiObject::Properties cpsapiObject::HiddenProperties = {};

cpsapiObject::cpsapiObject(CDataObject * pObject, const cpsapiObject::Type & type)
  : mpData()
{
  Data data;

  data.mpObject = pObject;
  data.mType = type;
  
  assertData(data);
}

cpsapiObject::cpsapiObject(const cpsapiObject & src)
  : mpData(src.mpData)
{}

// virtual
cpsapiObject::~cpsapiObject()
{}

cpsapiObject & cpsapiObject::operator= (const cpsapiObject & rhs)
{
  if (this != &rhs)
    mpData = rhs.mpData;

  return *this;
}

CDataObject * cpsapiObject::operator->() const
{
  return std::static_pointer_cast< Data >(mpData)->mpObject;
}

CDataObject * cpsapiObject::operator*() const
{
  return std::static_pointer_cast< Data >(mpData)->mpObject;
}

cpsapiObject::Type cpsapiObject::getType() const
{
  return std::static_pointer_cast< Data >(mpData)->mType;
}

cpsapiObject::operator bool() const
{
  return std::static_pointer_cast< Data >(mpData)->mpObject != nullptr;
}

CDataObject * cpsapiObject::getObject()
{
  return operator*();
}

CDataObject * cpsapiObject::getObject() const
{
  return operator*();
}

// virtual 
void cpsapiObject::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Object);
}

bool cpsapiObject::setProperty(const cpsapiObject::Property & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< cpsapiProperty::Type >(property), value, framework);
}

bool cpsapiObject::setProperty(const std::string & property, const cpsapiVariant & value, const std::string & framework)
{
  return setProperty(cpsapiProperty::Name.toEnum(property), value, CCore::FrameworkNames.toEnum(framework));
}

cpsapiVariant cpsapiObject::getProperty(const Property & property, const CCore::Framework & framework ) const
{
  return getProperty(static_cast< cpsapiProperty::Type >(property), framework);
}

cpsapiVariant cpsapiObject::getProperty(const std::string & property, const std::string & framework) const
{
  return getProperty(cpsapiProperty::Name.toEnum(property), CCore::FrameworkNames.toEnum(framework));
}

// virtual
bool cpsapiObject::setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  if (!isValidProperty<cpsapiObject>(property))
    return false;

  if (!operator bool())
    return false;

  bool success = false;

  switch (property)
    {
    case cpsapiProperty::Type::OBJECT_NAME:
      if (value.getType() == cpsapiVariant::Type::String)
        success = getObject()->setObjectName(value.toString());

      break;

    default:
      break;
    }

  return success;
}

// virtual 
cpsapiVariant cpsapiObject::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool()
      || !isValidProperty<cpsapiObject>(property))
    return cpsapiVariant();

  switch (property)
    {
    case cpsapiProperty::Type::OBJECT_NAME:
      return cpsapiVariant(getObject()->getObjectName());
      break;

    case cpsapiProperty::Type::DISPLAY_NAME:
      return getObject()->getObjectDisplayName();

    case cpsapiProperty::Type::CN:
      return static_cast< std::string >(getObject()->getCN());

    default:
      break;
    }

  return cpsapiVariant();
}
