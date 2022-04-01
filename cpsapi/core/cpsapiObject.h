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

#pragma once

#include <set>
#include <algorithm>
#include <memory>

#pragma GCC diagnostic push
#include <copasi/core/CDataVector.h>
#pragma GCC diagnostic pop

#include "cpsapi/core/cpsapiProperty.h"
#include "cpsapi/core/cpsapiVisitor.h"
#include "cpsapi/core/cpsapiObjectData.h"

class CDataObject;

#define DATA std::static_pointer_cast< Data >(mpData)
#define WRAPPED static_cast< wrapped * >(getObject())

CPSAPI_NAMESPACE_BEGIN

class cpsapiData;

/**
 * The cpsapiObject class is the base class for all COPASI CDataObjects exposed in the cpsapi.
 */
class cpsapiObject
{
  friend class cpsapiData;

public:
  static  const CCommonName Invalid;

  typedef std::set< cpsapiProperty::Type > Properties;
  typedef std::set< cpsapiReference::Type > References;

  typedef cpsapiObjectData Data;

public:
  /**
   * Enumeration of the exposed properties
   */
  enum class Property
  {
    NAME = cpsapiProperty::Type::NAME,
    OBJECT_UNIQUE_NAME = cpsapiProperty::Type::OBJECT_UNIQUE_NAME,
    CN = cpsapiProperty::Type::CN
  };

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * Static set of hidden properties
   */
  static const Properties HiddenProperties;

  /**
   * Enumeration of the exposed references
   */
  enum class Reference
  {
    NAME = cpsapiReference::Type::NAME,
    OBJECT_UNIQUE_NAME = cpsapiReference::Type::OBJECT_UNIQUE_NAME
  };

  /**
   * Static set of supported references
   */
  static const References SupportedReferences;

  /**
   * Static set of hidden references
   */
  static const References HiddenReferences;

  /**
   * The base class
   */
  typedef cpsapiObject self;

protected:
  /**
   * Specific  constructor
   * @param CDataObject * pObject (default: nullptr)
   * @param const Type & type (default: Type::Object)
   */
  cpsapiObject(CDataObject * pObject = nullptr, const cpsapiObjectData::Type & type = cpsapiObjectData::Type::Object);

public:
  /**
   * Copy constructor
   * @param const cpsapiObject & src
   */
  cpsapiObject(const cpsapiObject & src);

  /**
   * Destructor
   */
  virtual ~cpsapiObject();

  /**
   * Assignment operator =
   * @param const cpsapiObject & rhs 
   * @return cpsapiObject & 
   */
  cpsapiObject & operator=(const cpsapiObject & rhs);

  /**
   * Assignment operator =
   * @param CDataObject * pObject 
   * @return cpsapiObject & 
   */
  cpsapiObject & operator=(CDataObject * pObject);

  /**
   * Dereferencing operator ->
   * 
   * @return CDataObject* 
   */
  CDataObject * operator->() const;

  /**
   * Dereferencing operator *
   * 
   * @return CDataObject* 
   */
  CDataObject * operator*() const;

  /**
   * Comparison operator for equality 
   * 
   * @param const cpsapiObject & rhs 
   * @return bool equal 
   */
  bool operator==(const cpsapiObject & rhs) const;

  /**
   * Comparison operator for inequality 
   * 
   * @param const cpsapiObject & rhs 
   * @return bool notEqual 
   */
  bool operator!=(const cpsapiObject & rhs) const;

  /** 
   * Retrieve the type of the object
   * @return cpsapiObjectData::Type type
   * */
  cpsapiObjectData::Type getType() const;

  /**
   * Conversion to bool indicating whether the underlying COPASI CDataObject is accessible.
   * The underlying CDataObject might have been deleted, e.g., a species if the parent
   * compartment has been deleted
   */
  operator bool() const;

  /**
   * Check whether the object is valid 
   * @return bool valid
   */
  virtual bool isValid() const;

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor > void accept(Visitor & visitor);

protected:
  /**
   * Retrieve the pointer to the underlying COPASI CDataObject.
   * @return CDataObject * pObject
   */
  CDataObject * getObject() const;

public:
  /**
   * Set a property of the object to the provided value under the given framework.
   * The value must match the underlying value of the property.
   * The default framework is unspecified
   * @param const Property & property
   * @param const cpsapiData & value
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return bool success
   */
  bool setProperty(const Property & property, const cpsapiData & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  /**
   * Set a property of the object to the provided value under the given framework.
   * The property must be the string in cpsapiProperty::Names
   * The value must match the underlying value of the property.
   * The framework must be string in CCore::FrameworkNames
   * @param const std::string & property
   * @param const cpsapiData & value
   * @param const std::string & framework (default: "")
   * @return bool success
   */
  bool setProperty(const std::string & property, const cpsapiData & value, const std::string & framework = "");

  /**
   * Retrieve a property of the object to the provided value under the given framework.
   * The default framework is unspecified
   * @param const Property & property
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return cpsapiData property
   */
  cpsapiData getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

  /**
   * Retrieve a property of the object to the provided value under the given framework.
   * The property must be the string in cpsapiProperty::Names
   * The framework must be string in CCore::FrameworkNames
   * @param const std::string & property
   * @param const std::string & framework (default: "")
   * @return cpsapiData property
   */
  cpsapiData getProperty(const std::string & property, const std::string & framework = "") const;

  /**
   * Retrieve the effective list of supported properties
   * 
   * @tparam class CType cpsapiObject or a derived class
   * @return Properties SupportedProperties 
   */
  template < class CType >
  static Properties supportedProperties();

  /**
   * Check whether the property is supported  
   * 
   * @tparam class CType cpsapiObject or a derived class
   * @param const cpsapiProperty::Type & property
   * @return bool supported
   */
  template < class CType >
  static bool isSupportedProperty(const cpsapiProperty::Type & property);

  /**
   * Retrieve a data reference of the object under the given framework.
   * The default framework is unspecified
   * @param const Reference & reference
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return CCommonName
   */
CCommonName getDataCN(const Reference & reference, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

  /**
   * Retrieve a property of the object the given framework.
   * The property must be the string in cpsapiReferences::Names
   * The framework must be string in CCore::FrameworkNames
   * @param const std::string & reference
   * @param const std::string & framework (default: "")
   * @return CCommonName
   */
CCommonName getDataCN(const std::string & reference, const std::string & framework = "") const;

  /**
   * Retrieve the effective list of supported references
   * 
   * @tparam class CType cpsapiObject or a derived class
   * @return References SupportedReferences 
   */
  template < class CType >
  static References supportedReferences();

  /**
   * Check whether the reference is supported  
   * 
   * @tparam class CType cpsapiObject or a derived class
   * @param const cpsapiReference::Type & reference
   * @return bool supported
   */
  template < class CType >
  static bool isSupportedReference(const cpsapiReference::Type & reference);

protected:
  /**
   * Check whether the given property is supported by the class
   * 
   * @tparam class CType cpsapiObject or a derived class
   * @param const cpsapiProperty::Type & property
   * @return bool supported 
   */
  template < class CType >
  static bool isImplementedProperty(const cpsapiProperty::Type & property);

  /**
   * Check whether the given property is hidden by the class
   * 
   * @tparam class CType cpsapiObject or a derived class
   * @param const cpsapiProperty::Type & property
   * @return bool supported 
   */
  template < class CType >
  static bool isHiddenProperty(const cpsapiProperty::Type & property);

  /**
   * Set the property
   * 
   * @param const cpsapiProperty::Type & property 
   * @param const cpsapiData & value 
   * @param const CCore::Framework &framework 
   * @return bool success
   */
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework);

  /**
   * Retreive the property
   * 
   * @param const cpsapiProperty::Type & property 
   * @param const CCore::Framework &framework 
   * @return cpsapiData property
   */
  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const;

  /**
   * Check whether the given reference is supported by the class
   * 
   * @tparam class CType cpsapiObject or a derived class
   * @param const cpsapiReference::Type & reference
   * @return bool supported 
   */
  template < class CType >
  static bool isImplementedReference(const cpsapiReference::Type & reference);

  /**
   * Check whether the given reference is hidden by the class
   * 
   * @tparam class CType cpsapiObject or a derived class
   * @param const cpsapiReference::Type & reference
   * @return bool hidden 
   */
  template < class CType >
  static bool isHiddenReference(const cpsapiReference::Type & reference);

  /**
   * Retreive the data reference
   * 
   * @param const cpsapiReference::Type & reference 
   * @param const CCore::Framework &framework 
   * @return CCommonName
   */
  virtual CCommonName getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const;


  Data::Pointer mpData;
};

template < class Visitor >
void cpsapiObject::accept(Visitor & visitor)
{
  if (isValid())
    cpsapiVisitor::acceptIfVisitable(visitor, this); 
}

template <>
inline typename cpsapiObject::Properties cpsapiObject::supportedProperties< cpsapiObject >()
{
  return cpsapiObject::SupportedProperties;
}

template < class CType >
typename cpsapiObject::Properties cpsapiObject::supportedProperties()
{
  Properties All = supportedProperties< typename CType::base >();

  for (cpsapiProperty::Type property : CType::HiddenProperties)
    {
      All.erase(property);
    }

  All.insert(CType::SupportedProperties.begin(), CType::SupportedProperties.end());

  return All;
}

template < class CType >
bool cpsapiObject::isSupportedProperty(const cpsapiProperty::Type & property)
{
  Properties All = supportedProperties< CType >();

  return All.find(property) != All.end();
}

template < class CType >
bool cpsapiObject::isImplementedProperty(const cpsapiProperty::Type & property)
{
  return CType::SupportedProperties.find(property) != CType::SupportedProperties.end();
}

template < class CType >
bool cpsapiObject::isHiddenProperty(const cpsapiProperty::Type & property)
{
  return CType::HiddenProperties.find(property) != CType::HiddenProperties.end();
}

template <>
inline typename cpsapiObject::References cpsapiObject::supportedReferences< cpsapiObject >()
{
  return cpsapiObject::SupportedReferences;
}

template < class CType >
typename cpsapiObject::References cpsapiObject::supportedReferences()
{
  References All = supportedReferences< typename CType::base >();

  for (cpsapiReference::Type reference : CType::HiddenReferences)
    {
      All.erase(reference);
    }

  All.insert(CType::SupportedReferences.begin(), CType::SupportedReferences.end());

  return All;
}

template < class CType >
bool cpsapiObject::isSupportedReference(const cpsapiReference::Type & reference)
{
  References All = supportedReferences< CType >();

  return All.find(reference) != All.end();
}

template < class CType >
bool cpsapiObject::isImplementedReference(const cpsapiReference::Type & reference)
{
  return CType::SupportedReferences.find(reference) != CType::SupportedReferences.end();
}

template < class CType >
bool cpsapiObject::isHiddenReference(const cpsapiReference::Type & reference)
{
  return CType::HiddenReferences.find(reference) != CType::HiddenReferences.end();
}

CPSAPI_NAMESPACE_END

std::ostream & operator << (std::ostream &os, const CPSAPI_NAMESPACE_QUALIFIER cpsapiObject & object);

#include "cpsapi/core/cpsapiFactory.h"
#include "cpsapi/core/cpsapiData.h"
