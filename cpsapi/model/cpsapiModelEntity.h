// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#pragma once

#include "cpsapi/core/cpsapiContainer.h"

class CModelEntity;

CPSAPI_NAMESPACE_BEGIN

class cpsapiModelEntity: public cpsapiContainer
{
private:
  typedef cpsapiContainer base;

public:
  enum class Property
  {
    EXPRESSION = CData::Property::EXPRESSION,
    INITIAL_EXPRESSION = CData::Property::INITIAL_EXPRESSION,
    INITIAL_VALUE = CData::Property::INITIAL_VALUE,
    SIMULATION_TYPE = CData::Property::SIMULATION_TYPE,
    ADD_NOISE = CData::Property::ADD_NOISE,
    NOISE_EXPRESSION = CData::Property::NOISE_EXPRESSION,
    OBJECT_NAME = CData::Property::OBJECT_NAME
  };

  cpsapiModelEntity() = delete;

  cpsapiModelEntity(CModelEntity * pModelEntity);

  cpsapiModelEntity(const cpsapiModelEntity & src);

  virtual ~cpsapiModelEntity();

  bool set(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

protected:
  virtual bool set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework) override;

private:
  static Properties SupportedProperties;
};

CPSAPI_NAMESPACE_END