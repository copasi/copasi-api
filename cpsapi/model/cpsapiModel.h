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
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiGlobalQuantity.h"

#include <memory>

#include <copasi/model/CModel.h> 

class CModel;
class CCompartment;
class CMetab;
class CReaction;
class CModelValue;
class CEvent;

CPSAPI_NAMESPACE_BEGIN


class cpsapiModel: public cpsapiModelEntity
{
public:
  typedef cpsapiModelEntity base;

  enum class Property
  {
    INITIAL_VALUE = cpsapiProperty::Type::INITIAL_VALUE,
    OBJECT_NAME = cpsapiProperty::Type::OBJECT_NAME,
    DISPLAY_NAME = cpsapiProperty::Type::DISPLAY_NAME,
    CN = cpsapiProperty::Type::CN,
    UNIT = cpsapiProperty::Type::UNIT,
    VOLUME_UNIT = cpsapiProperty::Type::VOLUME_UNIT,
    AREA_UNIT = cpsapiProperty::Type::AREA_UNIT,
    LENGTH_UNIT = cpsapiProperty::Type::LENGTH_UNIT,
    TIME_UNIT = cpsapiProperty::Type::TIME_UNIT,
    QUANTITY_UNIT = cpsapiProperty::Type::QUANTITY_UNIT,
    MODEL_TYPE = cpsapiProperty::Type::MODEL_TYPE,
    AVOGADRO_NUMBER = cpsapiProperty::Type::AVOGADRO_NUMBER
  };

  static const Properties SupportedProperties;

  static const Properties HiddenProperties;

  cpsapiModel(CModel * pModel = nullptr);

  cpsapiModel(const cpsapiModel & src);

  virtual ~cpsapiModel();

  virtual void accept(cpsapiVisitor & visitor) override;

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
  
  cpsapiGlobalQuantity addGlobalQuantity(const std::string & name);

  bool deleteGlobalQuantity(const std::string & name = "");

  cpsapiGlobalQuantity globalQuantity(const std::string & name = "");

  std::vector< cpsapiGlobalQuantity > getGlobalQuantities() const;
  
  void deleteAllDependents(CDataContainer * pContainer);

  bool setProperty(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  CDataValue getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework) override;

  virtual CDataValue getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

private:
  cpsapiCompartment __compartment(const std::string & name) const;

  void updateDefaultCompartment(const cpsapiCompartment & compartment);

  cpsapiSpecies __species(const std::string & name, const std::string & compartment) const;

  cpsapiGlobalQuantity __globalQuantity(const std::string & name) const;

  void updateDefaultGlobalQuantity(const cpsapiGlobalQuantity & globalQuantity);
  
  void deleteDependents(const CDataObject::DataObjectSet & set);

  std::shared_ptr< cpsapiCompartment > mpDefaultCompartment;
  CReaction * mpDefaultReaction;
  std::shared_ptr< cpsapiGlobalQuantity > mpDefaultGlobalQuantity;
  CEvent * mpDefaultEvent;
};

CPSAPI_NAMESPACE_END