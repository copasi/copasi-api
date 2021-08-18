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
cpsapiObject::Map::iterator cpsapiObject::findOrInsert(const CDataObject * pObject)
{
  Map::iterator found = Manager.insert(std::make_pair(pObject, nullptr)).first;
  
  if (found->second == nullptr)
    found->second = new References;

  return found;
}

// static 
void cpsapiObject::deleted(const CDataObject * pObject)
{
  if (pObject == nullptr)
    return;

  Map::iterator found = Manager.find(const_cast< CDataObject * >(pObject));
  
  if (found == Manager.end())
    return;
  
  Map::iterator foundNull = findOrInsert(pObject);

  for (cpsapiObject * pPointer : *found->second)
    {
      pPointer->mpObject = nullptr;
      pPointer->mpReferences = foundNull->second;
    }

  foundNull->second->insert(found->second->begin(), found->second->end());

  Manager.erase(found);
}

// static
const cpsapiObject::Properties cpsapiObject::SupportedProperties = 
{
  cpsapiProperty::Type::OBJECT_NAME,
  cpsapiProperty::Type::DISPLAY_NAME,
  cpsapiProperty::Type::CN
};

// static
const cpsapiObject::Properties cpsapiObject::HiddenProperties = 
{};

cpsapiObject::cpsapiObject(CDataObject * pObject, const cpsapiObject::Type & typeId)
  : mpObject(pObject)
  , mpReferences(nullptr)
  , mType(typeId)
{
  addToReferences();
}

cpsapiObject::cpsapiObject(const cpsapiObject & src)
  : mpObject(src.mpObject)
  , mpReferences(nullptr)
  , mType(src.mType)
{
  addToReferences();
}

// virtual
cpsapiObject::~cpsapiObject()
{
  removeFromReferences();
}

cpsapiObject & cpsapiObject::operator= (const cpsapiObject & rhs)
{
  if (this == &rhs) return *this;

  removeFromReferences();

  mpObject = rhs.mpObject;

  addToReferences();

  return *this;
}

cpsapiObject & cpsapiObject::operator= (CDataObject * pObject)
{
  if (mpObject == pObject)
    return *this;

  removeFromReferences();

  mpObject = pObject;

  addToReferences();

  return *this;
}

CDataObject * cpsapiObject::operator->() const
{
  return mpObject;
}

CDataObject * cpsapiObject::operator*() const
{
  return mpObject;
}

cpsapiObject::References & cpsapiObject::references()
{
  static References Empty;

  if (mpReferences == nullptr)
    {
      return Empty;
    }

  return *mpReferences;
}

cpsapiObject::Type cpsapiObject::getType() const
{
  return mType;
}

cpsapiObject::operator bool() const
{
  return mpObject != nullptr;
}

void cpsapiObject::addToReferences()
{
  if (mpObject != nullptr)
    {
      mpReferences = findOrInsert(mpObject)->second;
      mpReferences->insert(this);
    }
}

void cpsapiObject::removeFromReferences()
{
  if (mpReferences != nullptr)
    {
      mpReferences->erase(this);

      if (mpReferences->empty())
        Manager.erase(mpObject);

      mpReferences = nullptr;
    }
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

  visitor.visit(this, Type::cpsapiObject);
}

bool cpsapiObject::setProperty(const cpsapiObject::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< cpsapiProperty::Type >(property), value, framework);
}

bool cpsapiObject::setProperty(const std::string & property, const CDataValue & value, const std::string & framework)
{
  return setProperty(cpsapiProperty::Name.toEnum(property), value, CCore::FrameworkNames.toEnum(framework));
}

CDataValue cpsapiObject::getProperty(const Property & property, const CCore::Framework & framework ) const
{
  return getProperty(static_cast< cpsapiProperty::Type >(property), framework);
}

CDataValue cpsapiObject::getProperty(const std::string & property, const std::string & framework) const
{
  return getProperty(cpsapiProperty::Name.toEnum(property), CCore::FrameworkNames.toEnum(framework));
}

// virtual
bool cpsapiObject::setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!isValidProperty<cpsapiObject>(property))
    return false;

  if (!operator bool())
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
CDataValue cpsapiObject::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool()
      || !isValidProperty<cpsapiObject>(property))
    return CDataValue();

  switch (property)
    {
    case cpsapiProperty::Type::OBJECT_NAME:
      return CDataValue(getObject()->getObjectName());
      break;

    case cpsapiProperty::Type::DISPLAY_NAME:
      return getObject()->getObjectDisplayName();

    case cpsapiProperty::Type::CN:
      return getObject()->getCN();

    default:
      break;
    }

  return CDataValue();
}
