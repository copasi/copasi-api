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

#include <copasi/model/CModelValue.h>

CPSAPI_NAMESPACE_BEGIN

class cpsapiModelEntity: public cpsapiContainer
{
public:
  typedef cpsapiContainer base;

  enum class Property
  {
    EXPRESSION = cpsapiProperty::Type::EXPRESSION,
    INITIAL_EXPRESSION = cpsapiProperty::Type::INITIAL_EXPRESSION,
    INITIAL_VALUE = cpsapiProperty::Type::INITIAL_VALUE,
    SIMULATION_TYPE = cpsapiProperty::Type::SIMULATION_TYPE,
    ADD_NOISE = cpsapiProperty::Type::ADD_NOISE,
    NOISE_EXPRESSION = cpsapiProperty::Type::NOISE_EXPRESSION,
    OBJECT_NAME = cpsapiProperty::Type::OBJECT_NAME,
    DISPLAY_NAME = cpsapiProperty::Type::DISPLAY_NAME,
    CN = cpsapiProperty::Type::CN
  };

  static const Properties SupportedProperties;

  cpsapiModelEntity() = delete;

protected:
  /**
   * Specific constructor
   * @param CModelEntity * pContainer
   * @param const Type & type
   */
  cpsapiModelEntity(CModelEntity * pModelEntity, const Type & type);

  /**
   * Copy constructor
   * @param const cpsapiModelEntity & src
   */
  cpsapiModelEntity(const cpsapiModelEntity & src);

public:
  virtual ~cpsapiModelEntity();

  virtual void accept(cpsapiVisitor & visitor) override;

  bool setProperty(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  CDataValue getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework) override;

  virtual CDataValue getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;
};

CPSAPI_NAMESPACE_END