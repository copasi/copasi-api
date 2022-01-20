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

#include "cpsapi/core/cpsapiGroup.h"

#pragma GCC diagnostic push
#include <copasi/utilities/CCopasiMethod.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_BEGIN

class cpsapiMethod: public cpsapiGroup
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property
  {
    VALUE = cpsapiProperty::Type::VALUE,
    OBJECT_NAME = cpsapiProperty::Type::OBJECT_NAME,
    DISPLAY_NAME = cpsapiProperty::Type::DISPLAY_NAME,
    CN = cpsapiProperty::Type::CN,
    TYPE = cpsapiProperty::Type::METHOD_TYPE
  };

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * The base class
   */
  typedef cpsapiGroup base;

  /**
   * The wrapped COPASI class
   */
  typedef CCopasiMethod wrapped;

  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
   */
  cpsapiMethod(wrapped * pWrapped = nullptr);

  /**
   * Copy constructor
   * @param const cpsapiMethod & src
   */
  cpsapiMethod(const cpsapiMethod & src);

  /**
   * Destructor
   */
  virtual ~cpsapiMethod();

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
