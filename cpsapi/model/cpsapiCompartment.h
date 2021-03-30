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

#include "cpsapi/model/cpsapiModelEntity.h"

class CCompartment;

CPSAPI_NAMESPACE_BEGIN

class cpsapiCompartment: public cpsapiModelEntity
{
private:
  typedef cpsapiModelEntity base;

public:
  enum class Property
  {
    INITIAL_INTENSIVE_VALUE = CData::Property::INITIAL_INTENSIVE_VALUE,
    EXPRESSION = CData::Property::EXPRESSION,
    INITIAL_EXPRESSION = CData::Property::INITIAL_EXPRESSION,
    INITIAL_VALUE = CData::Property::INITIAL_VALUE,
    SIMULATION_TYPE = CData::Property::SIMULATION_TYPE,
    ADD_NOISE = CData::Property::ADD_NOISE,
    NOISE_EXPRESSION = CData::Property::NOISE_EXPRESSION,
    OBJECT_NAME = CData::Property::OBJECT_NAME
  };

  cpsapiCompartment() = delete;

  cpsapiCompartment(CCompartment * pCompartment);

  cpsapiCompartment(const cpsapiCompartment & src);

  virtual ~cpsapiCompartment();

  bool set(const Property & property, const CDataValue & value);

protected:
  virtual bool set(const CData::Property & property, const CDataValue & value);

private:
  static Properties SupportedProperties;
};

CPSAPI_NAMESPACE_END