// BEGIN: Copyright 
// Copyright (C) 2022 by Pedro Mendes, Rector and Visitors of the 
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

#include "cpsapi/core/cpsapiContainer.h"

class copasiClass;

CPSAPI_NAMESPACE_BEGIN

class cpsapiTemplate : public cpsapiContainer
{
public:
  /**
   * Enumeration of the exposed properties
   */
  enum class Property
  {
    PARAMETER_VALUE = cpsapiProperty::Type::PARAMETER_VALUE,
    NAME = cpsapiProperty::Type::NAME,
    UNIQUE_NAME = cpsapiProperty::Type::UNIQUE_NAME,
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
    PARAMETER_VALUE = cpsapiProperty::Type::PARAMETER_VALUE,
    NAME = cpsapiReference::Type::NAME,
    UNIQUE_NAME = cpsapiReference::Type::UNIQUE_NAME
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
   * The class
   */
  typedef cpsapiTemplate self;

  /**
   * The base class
   */
  typedef cpsapiContainer base;

  /**
   * The wrapped COPASI class
   */
  typedef copasiClass wrapped;

  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
   * @param const Type & type (default: Type::cpsapiTemplate)
   */
  cpsapiTemplate(wrapped * pWrapped = nullptr, const Type & type = Type::Parameter);

  /**
   * Destructor
   */
  virtual ~cpsapiTemplate();

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor > void accept(Visitor & visitor);

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
   * The value must match the underlying value of the property.
   * The default framework is unspecified
   * @param const Property & property
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return cpsapiData property
   */
  cpsapiData getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

  /**
   * Retrieve a property of the object to the provided value under the given framework.
   * The default framework is unspecified
   * @param const Reference & reference
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return CCommonName
   */
  CCommonName getDataCN(const Reference & reference, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  /**
   * Set the property
   * 
   * @param const cpsapiProperty::Type & property 
   * @param const cpsapiData & value 
   * @param const CCore::Framework &framework 
   * @return bool success
   */
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework) override;

  /**
   * Retreive the property
   * 
   * @param const cpsapiProperty::Type & property 
   * @param const CCore::Framework &framework 
   * @return cpsapiData property
   */
  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

  /**
   * Retreive the data reference
   * 
   * @param const cpsapiReference::Type & reference 
   * @param const CCore::Framework &framework 
   * @return CCommonName
   */
  virtual CCommonName getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const override;
};

template< class Visitor >
void cpsapiTemplate::accept(Visitor & visitor)
{
  if (isValid())
    {
      cpsapiVisitorBase::acceptIfVisitable(visitor, this); 
      base::accept(visitor);
    }
}

CPSAPI_NAMESPACE_END