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

#include "cpsapi/model/cpsapiModelEntity.h"

class CMetab;

CPSAPI_NAMESPACE_BEGIN

class cpsapiSpecies: public cpsapiModelEntity
{
public:
  typedef cpsapiModelEntity base;

  enum class Property
  {
    EXPRESSION = CData::Property::EXPRESSION,
    INITIAL_EXPRESSION = CData::Property::INITIAL_EXPRESSION,
    INITIAL_VALUE = CData::Property::INITIAL_VALUE,
    SIMULATION_TYPE = CData::Property::SIMULATION_TYPE,
    ADD_NOISE = CData::Property::ADD_NOISE,
    NOISE_EXPRESSION = CData::Property::NOISE_EXPRESSION,
    OBJECT_NAME = CData::Property::OBJECT_NAME,
    UNIT = CData::Property::UNIT
  };

  static const Properties SupportedProperties;

  cpsapiSpecies(CMetab * pSpecies = nullptr);

  cpsapiSpecies(const cpsapiSpecies & src);

  virtual ~cpsapiSpecies();

  virtual void accept(cpsapiVisitor & visitor) override;

  bool set(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  CDataValue get(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework) override;

  virtual CDataValue get(const CData::Property & property, const CCore::Framework & framework) const override;
};

CPSAPI_NAMESPACE_END