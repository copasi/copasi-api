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

#include "cpsapi/core/cpsapiContainer.h"

#include <memory>

class CCopasiParameter;

CPSAPI_NAMESPACE_BEGIN

class cpsapiParameter : public cpsapiContainer
{
public:
  typedef cpsapiContainer base;

  /**
   * Enumeration of the exposed properties
   */ 
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

  static const Properties SupportedProperties;

  cpsapiParameter(CCopasiParameter * pParameter, const Type & typeId);

  cpsapiParameter(const cpsapiParameter & src);

  virtual ~cpsapiParameter();

  virtual void accept(cpsapiVisitor & visitor) override;

  bool setProperty(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  CDataValue getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework) override;

  virtual CDataValue getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

  std::shared_ptr< cpsapiParameter > mpDefaultParameter;
};

CPSAPI_NAMESPACE_END