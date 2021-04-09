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

#include <copasi/model/CModel.h> 

class CModel;
class CCompartment;
class CMetab;
class CReaction;
class CModelValue;
class CEvent;

CPSAPI_NAMESPACE_BEGIN

class cpsapiCompartment;
class cpsapiSpecies;

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

  cpsapiModel(CModel * pModel = nullptr);

  cpsapiModel(const cpsapiModel & src);

  virtual ~cpsapiModel();

  void beginTransaction() const;

  void endTransaction() const;

  bool synchronize(std::set< const CDataObject * > & changedObjects);

  cpsapiCompartment addCompartment(const std::string & name);

  bool deleteCompartment(const std::string & name = "");

  cpsapiCompartment compartment(const std::string & name = "");

  std::vector< cpsapiCompartment > getCompartments() const;
  
  cpsapiSpecies addSpecies(const std::string & name, const std::string & compartment = "");

  bool deleteSpecies(const std::string & name = "", const std::string & compartment = "");

  cpsapiSpecies species(const std::string & name = "", const std::string & compartment = "");

  std::vector< cpsapiSpecies > getSpecies() const;
  
  void deleteAllDependents(CDataContainer * pContainer);

private:
  CCompartment * __compartment(const std::string & name) const;

  CMetab * __species(const std::string & name, const std::string & compartment) const;

  template < class CType > void deleteDependents(CType *& pDefault, const CDataObject::DataObjectSet & set);

  CCompartment * mpDefaultCompartment;
  CReaction * mpDefaultReaction;
  CModelValue * mpDefaultGlobalQuantity;
  CEvent * mpDefaultEvent;
};

template < class CType >
void cpsapiModel::deleteDependents(CType *& pDefault, const CDataObject::DataObjectSet & set)
{
  CDataObject::DataObjectSet::const_iterator it = set.begin();
  CDataObject::DataObjectSet::const_iterator end = set.end();

  for (; it != end; ++it)
    {
      if (pDefault != nullptr
          && dynamic_cast< const CType * >(*it) != pDefault)
        pDefault = nullptr;

      cpsapiPointer::deleted(*it);
      delete *it;
    }
}

CPSAPI_NAMESPACE_END