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

class CModel;
class CCompartment;
class CMetab;
class CReaction;
class CModelValue;
class CEvent;

CPSAPI_NAMESPACE_BEGIN

class cpsapiCompartment;

class cpsapiModel: public cpsapiModelEntity
{
private:
  typedef cpsapiModelEntity base;

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

  cpsapiModel() = delete;

  cpsapiModel(CModel * pModel);

  cpsapiModel(const cpsapiModel & src);

  virtual ~cpsapiModel();

  void beginTransaction() const;

  void endTransaction() const;

  bool synchronize(const std::set< CDataObject * > & changedObjects);

  bool addCompartment(const std::string & name);

  bool deleteCompartment(const std::string & name);

  bool selectCompartment(const std::string & name);

  CCompartment * compartment(const std::string & name = "");

  std::vector< cpsapiCompartment > getCompartments() const;
  

private:
  void changeCompartment(CCompartment * pCompartment);
  void assertDefaultCompartment();

  CCompartment * pDefaultCompartment;
  CMetab * pDefaultSpecies;
  CReaction * pDefaultReaction;
  CModelValue * pDefaultGlobalQuantity;
  CEvent * pDefaultEvent;
};

CPSAPI_NAMESPACE_END