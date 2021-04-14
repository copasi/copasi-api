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
    OBJECT_NAME = CData::Property::OBJECT_NAME,
    UNIT = CData::Property::UNIT,
    VOLUME_UNIT = CData::Property::VOLUME_UNIT,
    AREA_UNIT = CData::Property::AREA_UNIT,
    LENGTH_UNIT = CData::Property::LENGTH_UNIT,
    TIME_UNIT = CData::Property::TIME_UNIT,
    QUANTITY_UNIT = CData::Property::QUANTITY_UNIT,
    MODEL_TYPE = CData::Property::MODEL_TYPE,
    AVOGADRO_NUMBER = CData::Property::AVOGADRO_NUMBER
  };

  cpsapiModel(CModel * pModel = nullptr);

  cpsapiModel(const cpsapiModel & src);

  virtual ~cpsapiModel();

  void beginTransaction() const;

  void endTransaction() const;

  bool synchronize(std::set< const CDataObject * > & changedObjects, const CCore::Framework & framework);

  bool compile();

  cpsapiCompartment addCompartment(const std::string & name);

  bool deleteCompartment(const std::string & name = "");

  cpsapiCompartment compartment(const std::string & name = "");

  std::vector< cpsapiCompartment > getCompartments() const;
  
  cpsapiSpecies addSpecies(const std::string & name, const std::string & compartment = "");

  bool deleteSpecies(const std::string & name = "", const std::string & compartment = "");

  cpsapiSpecies species(const std::string & name = "", const std::string & compartment = "");

  std::vector< cpsapiSpecies > getSpecies() const;
  
  void deleteAllDependents(CDataContainer * pContainer);

  bool set(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  CDataValue get(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework) override;

  virtual CDataValue get(const CData::Property & property, const CCore::Framework & framework) const override;

private:
  CCompartment * __compartment(const std::string & name) const;

  CMetab * __species(const std::string & name, const std::string & compartment) const;

  template < class CType > void deleteDependents(CType *& pDefault, const CDataObject::DataObjectSet & set);

  CCompartment * mpDefaultCompartment;
  CReaction * mpDefaultReaction;
  CModelValue * mpDefaultGlobalQuantity;
  CEvent * mpDefaultEvent;

  static Properties SupportedProperties;
};

template < class CType >
void cpsapiModel::deleteDependents(CType *& pDefault, const CDataObject::DataObjectSet & set)
{
  std::for_each(set.begin(), set.end(), [&pDefault](const CDataObject * pObject) {
    if (pDefault != nullptr
        && dynamic_cast< const CType * >(pObject) != pDefault)
      pDefault = nullptr;

    cpsapiPointer::deleted(pObject);
    delete pObject;
  });
}

CPSAPI_NAMESPACE_END