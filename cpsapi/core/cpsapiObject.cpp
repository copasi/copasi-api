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

#include "cpsapi/core/cpsapiObject.h"
#include "cpsapi/core/cpsapiValue.h"

#pragma GCC diagnostic push
#include <copasi/core/CDataObject.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static 
cpsapiObject::Map cpsapiObject::Manager;

// static
const CCommonName cpsapiObject::Invalid;

// static
void cpsapiObject::release()
{
  Map::iterator it = Manager.begin();
  Map::iterator end = Manager.end();

  for (; it != end; ++it)
    it->second.reset();
}

// static 
void cpsapiObject::erase(const CDataObject * pObject)
{
  if (pObject == nullptr)
    return;

  Map::iterator found = Manager.find(const_cast< CDataObject * >(pObject));
  
  if (found == Manager.end())
    return;

  
  if (found->second.get() != nullptr)
    std::static_pointer_cast< Data >(found->second)->mpObject = nullptr;

  Manager.erase(pObject);
}

// static
void cpsapiObject::deleted(const CDataObject * pObject)
{
  if (dynamic_cast< const CDataContainer * >(pObject))
    for (const CDataObject * pChildObject : static_cast< const CDataContainer * >(pObject)->getObjects())
      deleted(pChildObject);

  erase(pObject);
}

// static
const CEnumAnnotation< std::string, cpsapiObject::Type > cpsapiObject::TypeName(
{
  "Object",
  "Container",
  "Vector",
  "ModelEntity",
  "Value",
  "Model",
  "Compartment",
  "Species",
  "GlobalQuantity",
  "Reaction",
  "ReactionParameter",
  "DataModel",
  "Parameter",
  "Group",
  "Method",
  "Problem",
  "Task"
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

// static
const cpsapiObject::References cpsapiObject::SupportedReferences =
{
  cpsapiReference::Type::OBJECT_NAME,
  cpsapiProperty::Type::DISPLAY_NAME
};

// static
const cpsapiObject::References cpsapiObject::HiddenReferences = {};

std::ostream & operator << (std::ostream &os, const cpsapiObject & object)
{
  os << cpsapiObject::TypeName[object.getType()] << ": " << object.getProperty(cpsapiObject::Property::OBJECT_NAME).toString();

  return os;
}

cpsapiObject::cpsapiObject(CDataObject * pObject, const cpsapiObject::Type & type)
  : mpData()
{
  Data data;

  data.mpObject = pObject;
  data.mType = type;

  if (pObject == nullptr)
    {
      mpData = std::make_shared< Data >(data);
    }
  else
    {
      Map::iterator found = Manager.insert(std::make_pair(data.mpObject, nullptr)).first;

      if (!std::dynamic_pointer_cast< Data >(found->second))
        found->second = std::make_shared< Data >(data);

      mpData = found->second;
    }
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
  return DATA->mpObject;
}

CDataObject * cpsapiObject::operator*() const
{
  return DATA->mpObject;
}

bool cpsapiObject::operator==(const cpsapiObject & rhs) const
{
  return DATA == std::static_pointer_cast< Data >(rhs.mpData);
}

bool cpsapiObject::operator!=(const cpsapiObject & rhs) const
{
  return DATA != std::static_pointer_cast< Data >(rhs.mpData);
}

cpsapiObject::Type cpsapiObject::getType() const
{
  return DATA->mType;
}

cpsapiObject::operator bool() const
{
  return isValid();
}

// virtual 
bool cpsapiObject::isValid() const
{
  return DATA->mpObject != nullptr;
}

CDataObject * cpsapiObject::getObject() const
{
  return DATA->mpObject;
}

// virtual 
void cpsapiObject::accept(cpsapiVisitor & visitor)
{
  if (!isValid())
    return;

  visitor.visit(this, Type::Object);
}

bool cpsapiObject::setProperty(const cpsapiObject::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< cpsapiProperty::Type >(property), value, framework);
}

bool cpsapiObject::setProperty(const std::string & property, const cpsapiData & value, const std::string & framework)
{
  return setProperty(cpsapiProperty::Name.toEnum(property), value, CCore::FrameworkNames.toEnum(framework));
}

cpsapiData cpsapiObject::getProperty(const cpsapiObject::Property & property, const CCore::Framework & framework ) const
{
  return getProperty(static_cast< cpsapiProperty::Type >(property), framework);
}

cpsapiData cpsapiObject::getProperty(const std::string & property, const std::string & framework) const
{
  return getProperty(cpsapiProperty::Name.toEnum(property), CCore::FrameworkNames.toEnum(framework));
}

CCommonName cpsapiObject::getDataCN(const cpsapiObject::Reference & reference, const CCore::Framework & framework ) const
{
  return getDataCN(static_cast< cpsapiReference::Type >(reference), framework);
}

CCommonName cpsapiObject::getDataCN(const std::string & reference, const std::string & framework) const
{
  return getDataCN(cpsapiReference::Name.toEnum(reference), CCore::FrameworkNames.toEnum(framework));
}

// virtual
bool cpsapiObject::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & /* framework */)
{
  if (!isValid()
      || isHiddenProperty< cpsapiObject >(property)
      || !isImplementedProperty< cpsapiObject >(property))
    return false;

  bool success = false;

  switch (property)
    {
    case cpsapiProperty::Type::OBJECT_NAME:
      if (value.getType() == cpsapiData::Type::String)
        success = getObject()->setObjectName(value.toString());

      break;

    default:
      break;
    }

  return success;
}

// virtual 
cpsapiData cpsapiObject::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & /* framework */) const
{
  if (!isValid()
      || isHiddenProperty< cpsapiObject >(property)
      || !isImplementedProperty< cpsapiObject >(property))
    return cpsapiData();

  switch (property)
    {
    case cpsapiProperty::Type::OBJECT_NAME:
      return getObject()->getObjectName();
      break;

    case cpsapiProperty::Type::DISPLAY_NAME:
      return getObject()->getObjectDisplayName();

    case cpsapiProperty::Type::CN:
      return CCommonName (getObject()->getCN());

    default:
      break;
    }

  return cpsapiData();
}

// virtual 
CCommonName cpsapiObject::getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & /* framework */) const
{
  if (!isValid()
      || isHiddenReference< cpsapiObject >(reference)
      || !isImplementedReference< cpsapiObject >(reference))
    return Invalid;

  switch (reference)
    {
    case cpsapiReference::Type::OBJECT_NAME:
      return getObject()->getObject(std::string("Property=Name"))->getDataObject()->getCN();
      break;

    case cpsapiReference::Type::DISPLAY_NAME:
      return getObject()->getObject(std::string("Property=DisplayName"))->getDataObject()->getCN();

    default:
      break;
    }

  return Invalid;
}