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
#include <copasi/undo/CData.h>
#include <copasi/core/CDataVector.h>

#include "cpsapi/core/cpsapiVisitor.h"
#include "cpsapi/core/cpsapiPointer.h"

class CDataObject;

CPSAPI_NAMESPACE_BEGIN

/**
 * The cpsapiObject class is the base class for all COPASI CDataObjects exposed in the cpsapi.
 */
class cpsapiObject
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property
  {
    OBJECT_NAME = CData::Property::OBJECT_NAME
  };

  /**
   * Default constructor
   * @param CDataObject * pObject (default: nullptr)
   */
  cpsapiObject(CDataObject * pObject = nullptr);

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
  virtual void accept(cpsapiVisitor& visitor);

  /**
   * Retrieve the pointer to the underlying COPASI CDataObject. 
   * @return CDataObject * pObject
   */
  CDataObject * getObject();

  /**
   * Retrieve the pointer to the underlying COPASI CDataObject. 
   * @return const CDataObject * pObject
   */
  const CDataObject * getObject() const;

  /**
   * Set a property of the object to the provided value under the given framework.
   * The value must match the underlying value of the property.
   * The default framework is unspecified 
   * @param const Property & property
   * @param const CDataValue & value
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return bool success
   */
  bool set(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  /**
   * Set a property of the object to the provided value under the given framework.
   * The property must be the string in CData::PropertyNames
   * The value must match the underlying value of the property.
   * The framework must be string in CCore::FrameworkNames 
   * @param const std::string & property
   * @param const CDataValue & value
   * @param const std::string & framework (default: "")
   * @return bool success
   */
  bool set(const std::string & property, const CDataValue & value, const std::string & framework = "");

  /**
   * Retrieve a property of the object to the provided value under the given framework.
   * The default framework is unspecified 
   * @param const Property & property
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return CDataValue value
   */
  CDataValue get(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

  /**
   * Retrieve a property of the object to the provided value under the given framework.
   * The property must be the string in CData::PropertyNames
   * The framework must be string in CCore::FrameworkNames 
   * @param const std::string & property
   * @param const std::string & framework (default: "")
   * @return CDataValue value
   */
  CDataValue get(const std::string & property, const std::string & framework = "") const;

protected:
  typedef std::set< CData::Property > Properties;

  bool isValidProperty(const std::string & property) const;

  bool isValidProperty(const CData::Property & property) const;

  virtual bool set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework);

  virtual CDataValue get(const CData::Property & property, const CCore::Framework & framework) const;

  cpsapiPointer mpObject;
  Properties * mpSupportedProperties;

private:
  static Properties SupportedProperties;
};

template < typename Target, class SourceVector > std::vector< Target > convertVector(SourceVector  &src)
{
  std::vector< Target > Result(src.size());
  typename SourceVector::iterator itSrc = src.begin();

  std::for_each(Result.begin(), Result.end(), [&itSrc](Target & target){
    target = &*itSrc;
    ++itSrc;
  });

  return Result;
}


CPSAPI_NAMESPACE_END
