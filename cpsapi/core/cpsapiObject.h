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

#pragma once

#include <set>
#include <algorithm>
#include <memory>

#include <copasi/core/CDataVector.h>

#include "cpsapi/core/cpsapiProperty.h"

class CDataObject;

#define DATA std::static_pointer_cast< Data >(mpData)
#define WRAPPED static_cast< wrapped * >(getObject())

CPSAPI_NAMESPACE_BEGIN

class cpsapiVisitor;
class cpsapiVariant;


/**
 * The cpsapiObject class is the base class for all COPASI CDataObjects exposed in the cpsapi.
 */
class cpsapiObject
{
  friend class cpsapiVariant;

public:
  static void release();

  static void erase(const CDataObject * pObject);

  static void deleted(const CDataObject * pObject);

  enum struct Type
  {
    Object,
    Container,
    Vector,
    ModelEntity,
    Value,
    Model,
    Compartment,
    Species,
    GlobalQuantity,
    Reaction,
    ReactionParameter,
    DataModel,
    Parameter,
    Group,
    __SIZE
  };

  static const CEnumAnnotation< std::string, Type > TypeName;

  typedef std::set< cpsapiProperty::Type > Properties;

protected:
  class Data 
  {
  public:
    virtual ~Data() {}

    CDataObject * mpObject;
    Type mType;
  };

  typedef std::shared_ptr< Data > DataPointer;

  typedef std::map< const CDataObject *, DataPointer > Map;

  static Map Manager;

public:
  /**
   * Enumeration of the exposed properties
   */
  enum class Property
  {
    OBJECT_NAME = cpsapiProperty::Type::OBJECT_NAME,
    DISPLAY_NAME = cpsapiProperty::Type::DISPLAY_NAME,
    CN = cpsapiProperty::Type::CN
  };

  static const Properties SupportedProperties;

  static const Properties HiddenProperties;

  DataPointer mpData;

  /**
   * Deleted default constructor
   */
  cpsapiObject() = delete;

protected:
  /**
   * Specific  constructor
   * @param CDataObject * pObject (default: nullptr)
   * @param const Type & type
   */
  cpsapiObject(CDataObject * pObject, const Type & type);

  /**
   * Copy constructor
   * @param const cpsapiObject & src
   */
  cpsapiObject(const cpsapiObject & src);

public:
  /**
   * Destructor
   */
  virtual ~cpsapiObject();

  cpsapiObject & operator=(const cpsapiObject & rhs);

  cpsapiObject & operator=(CDataObject * pObject);

  CDataObject * operator->() const;

  CDataObject * operator*() const;

  Type getType() const;

  /**
   * Conversion to bool indicating whether the underlying COPASI CDataObject is accessible.
   * The underlying CDataObject might have been deleted, e.g., a species if the parent
   * compartment has been deleted
   */
  operator bool() const;

  /**
   * A a virtual accept method implementing a visitor pattern.
   * Valid cpsapiObjects will call:  visitor.visit(*this)
   * Visitors have read and write access to the object.
   * @param accept(cpsapiVisitor& visitor)
   */
  virtual void accept(cpsapiVisitor & visitor);

  /**
   * Retrieve the pointer to the underlying COPASI CDataObject.
   * @return CDataObject * pObject
   */
  CDataObject * getObject() const;

  /**
   * Set a property of the object to the provided value under the given framework.
   * The value must match the underlying value of the property.
   * The default framework is unspecified
   * @param const Property & property
   * @param const cpsapiVariant & value
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return bool success
   */
  bool setProperty(const Property & property, const cpsapiVariant & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  /**
   * Set a property of the object to the provided value under the given framework.
   * The property must be the string in cpsapiProperty::Names
   * The value must match the underlying value of the property.
   * The framework must be string in CCore::FrameworkNames
   * @param const std::string & property
   * @param const cpsapiVariant & value
   * @param const std::string & framework (default: "")
   * @return bool success
   */
  bool setProperty(const std::string & property, const cpsapiVariant & value, const std::string & framework = "");

  /**
   * Retrieve a property of the object to the provided value under the given framework.
   * The default framework is unspecified
   * @param const Property & property
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return cpsapiVariant value
   */
  cpsapiVariant getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

  /**
   * Retrieve a property of the object to the provided value under the given framework.
   * The property must be the string in cpsapiProperty::Names
   * The framework must be string in CCore::FrameworkNames
   * @param const std::string & property
   * @param const std::string & framework (default: "")
   * @return cpsapiVariant value
   */
  cpsapiVariant getProperty(const std::string & property, const std::string & framework = "") const;

  template < class CType >
  static Properties AllSupportedProperties();

protected:
  template < class CType >
  static bool isValidProperty(const cpsapiProperty::Type & property);

  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework);

  virtual cpsapiVariant getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const;

  template < class CData >
  void assertData(const CData & data);
};

template <>
inline cpsapiObject::Properties cpsapiObject::AllSupportedProperties< cpsapiObject >()
{
  return cpsapiObject::SupportedProperties;
}

template < class CType >
cpsapiObject::Properties cpsapiObject::AllSupportedProperties()
{
  Properties All = AllSupportedProperties< typename CType::base >();

  for (cpsapiProperty::Type property : CType::HiddenProperties)
    {
      All.erase(property);
    }

  All.insert(CType::SupportedProperties.begin(), CType::SupportedProperties.end());

  return All;
}

template < class CType >
bool cpsapiObject::isValidProperty(const cpsapiProperty::Type & property)
{
  return CType::SupportedProperties.find(property) != CType::SupportedProperties.end();
}

template < class CData >
void cpsapiObject::assertData(const CData & data)
{
  if (data.mpObject == nullptr)
    {
      mpData = std::make_shared< CData >(data);
      return;
    }

  Map::iterator found = Manager.insert(std::make_pair(data.mpObject, nullptr)).first;

  if (!std::dynamic_pointer_cast< CData >(found->second))
    found->second = std::make_shared< CData >(data);

  mpData = found->second;
}

CPSAPI_NAMESPACE_END

std::ostream & operator << (std::ostream &os, const CPSAPI_NAMESPACE_QUALIFIER cpsapiObject & object);

#include "cpsapi/core/cpsapiFactory.h"
#include "cpsapi/core/cpsapiVariant.h"
#include "cpsapi/core/cpsapiVisitor.h"
