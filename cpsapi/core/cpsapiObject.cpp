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
const CCommonName cpsapiObject::Invalid;


// static
const cpsapiObject::Properties cpsapiObject::SupportedProperties =
{
  cpsapiProperty::Type::NAME,
  cpsapiProperty::Type::OBJECT_UNIQUE_NAME,
  cpsapiProperty::Type::CN
};

// static
const cpsapiObject::Properties cpsapiObject::HiddenProperties = {};

// static
const cpsapiObject::References cpsapiObject::SupportedReferences =
{
  cpsapiReference::Type::NAME,
  cpsapiProperty::Type::OBJECT_UNIQUE_NAME
};

// static
const cpsapiObject::References cpsapiObject::HiddenReferences = {};

std::ostream & operator << (std::ostream &os, const cpsapiObject & object)
{
  os << cpsapiObjectData::TypeName[object.getType()] << ": " << object.getProperty(cpsapiObject::Property::NAME).toString();

  return os;
}

cpsapiObject::cpsapiObject(CDataObject * pObject, const cpsapiObjectData::Type & type)
  : mpData(std::make_shared< Data >(pObject, type))
{
  Data::assertDataType< cpsapiObject >(mpData);
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

cpsapiObjectData::Type cpsapiObject::getType() const
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
    case cpsapiProperty::Type::NAME:
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
    case cpsapiProperty::Type::NAME:
      return getObject()->getObjectName();
      break;

    case cpsapiProperty::Type::OBJECT_UNIQUE_NAME:
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
    case cpsapiReference::Type::NAME:
      return getObject()->getObject(std::string("Property=Name"))->getDataObject()->getCN();
      break;

    case cpsapiReference::Type::OBJECT_UNIQUE_NAME:
      return getObject()->getObject(std::string("Property=DisplayName"))->getDataObject()->getCN();

    default:
      break;
    }

  return Invalid;
}