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

#include "cpsapi/core/cpsapiContainer.h"

#include <memory>

class CCopasiParameter;

CPSAPI_NAMESPACE_BEGIN

class cpsapiParameter : public cpsapiContainer
{
public:
  /**
   * Enumeration of the exposed properties
   */
  enum class Property
  {
    PARAMETER_VALUE = cpsapiProperty::Type::PARAMETER_VALUE,
    OBJECT_NAME = cpsapiProperty::Type::OBJECT_NAME,
    DISPLAY_NAME = cpsapiProperty::Type::DISPLAY_NAME,
    CN = cpsapiProperty::Type::CN
  };

 /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * The base class
   */
  typedef cpsapiContainer base;

  /**
   * The wrapped COPASI class
   */
  typedef CCopasiParameter wrapped;

  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
   * @param const Type & type (default: Type::cpsapiParameter)
   */
  cpsapiParameter(wrapped * pWrapped = nullptr, const Type & type = Type::Parameter);

  /**
   * Copy constructor
   * @param const cpsapiContainer & src
   */
  cpsapiParameter(const cpsapiParameter & src);

  /**
   * Destructor
   */
  virtual ~cpsapiParameter();

  /**
   * Accept a visitor
   * @param cpsapiVisitor & visitor
   */
  virtual void accept(cpsapiVisitor & visitor) override;

  bool setProperty(const Property & property, const cpsapiData & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  cpsapiData getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework) override;

  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;
};

CPSAPI_NAMESPACE_END