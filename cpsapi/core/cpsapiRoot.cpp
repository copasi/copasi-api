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

#define COPASI_MAIN

#include <copasi/core/CRootContainer.h>
#include <copasi/CopasiDataModel/CDataModel.h>
#include <copasi/core/CDataVector.h>

#include "cpsapi/core/cpsapiRoot.h"
#include "cpsapi/core/cpsapiDataModel.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"

CPSAPI_NAMESPACE_USE

// static 
std::map< std::string, CDataModel * > cpsapi::DataModels;

// static 
cpsapiDataModel cpsapi::DefaultDataModel(nullptr);

// static 
CFunction * cpsapi::pDefaultFunction = nullptr;

// static 
CUnitDefinition * cpsapi::pDefaultUnitDefinition = nullptr;

// static 
cpsapiDataModel & cpsapi::addDataModel(const std::string & name)
{
  if (DataModels.find(name) != DataModels.end())
    {
      DefaultDataModel = cpsapiDataModel(nullptr);
      return DefaultDataModel;
    }

  CDataModel * pDefaultDataModel = CRootContainer::addDatamodel();
  DataModels.insert(std::make_pair(name, pDefaultDataModel));

  DefaultDataModel = cpsapiDataModel(pDefaultDataModel);

  // We start with the names being the same
  model().getObject()->setObjectName(name);
  
  return DefaultDataModel;
}

// static
bool cpsapi::deleteDataModel(const std::string & name)
{
  std::map< std::string, CDataModel * >::iterator found = DataModels.find(name);

  if (found == DataModels.end())
    return false;

  if (DefaultDataModel.getObject() == found->second)
    DefaultDataModel = cpsapiDataModel(nullptr);

  delete found->second;
  DataModels.erase(found);

  return true;
}

// static
cpsapiDataModel & cpsapi::dataModel(const std::string & name)
{
  if (name.empty())
    {
      if (!DefaultDataModel
          && CRootContainer::getDatamodelList()->empty())
        return addDataModel("default model");

      return DefaultDataModel;
    }

  std::map< std::string, CDataModel * >::iterator found = DataModels.find(name);

  if (found != DataModels.end())
    {
      DefaultDataModel = cpsapiDataModel(found->second);
    }
  else
    {
      DefaultDataModel = cpsapiDataModel(nullptr);
    }

  return DefaultDataModel;
}

// static 
std::vector< cpsapiDataModel > cpsapi::getDataModels()
{
  return convertVector< cpsapiDataModel >(*CRootContainer::getDatamodelList());
}

// static
const std::set< std::string > cpsapi::listModelNames()
{
  std::set< std::string > Set;

  std::for_each(DataModels.begin(), DataModels.end(), [&Set](const std::pair< std::string, CDataModel * > & mapIterator) {
    Set.insert(mapIterator.first);
  });

  return Set;
}

// static
bool cpsapi::loadFromFile(const std::string & fileName, const std::string & modelName)
{
  return dataModel(modelName).loadFromFile(fileName);
}

// static
bool cpsapi::loadFromString(const std::string & content, const std::string & referenceDir, const std::string & modelName)
{
  return dataModel(modelName).loadFromString(content, referenceDir);
}

// static
cpsapiModel & cpsapi::model(const std::string & name)
{
  return dataModel(name).model();
}

// static
void cpsapi::beginTransaction(const std::string & name)
{
  dataModel(name).beginTransaction();
}

// static
void cpsapi::endTransaction(const std::string & name)

{
  dataModel(name).endTransaction();
}

// static
bool cpsapi::addCompartment(const std::string & name, const std::string & modelName)
{
  return dataModel(modelName).addCompartment(name);
}

// static
bool cpsapi::deleteCompartment(const std::string & name, const std::string & modelName)
{
  return dataModel(modelName).deleteCompartment(name);
}

// static 
cpsapiCompartment cpsapi::compartment(const std::string & name, const std::string & modelName)
{
  return dataModel(modelName).compartment(name);
}

// static 
std::vector< cpsapiCompartment > cpsapi::getCompartments(const std::string & modelName)
{
  return dataModel(modelName).getCompartments();
}

// static
bool cpsapi::addSpecies(const std::string & name, const std::string & compartmentName, const std::string & modelName)
{
  return dataModel(modelName).addSpecies(name, compartmentName);
}

// static
bool cpsapi::deleteSpecies(const std::string & name, const std::string & compartmentName, const std::string & modelName)
{
  return dataModel(modelName).deleteSpecies(name, compartmentName);
}

// static 
cpsapiSpecies cpsapi::species(const std::string & name, const std::string & compartmentName, const std::string & modelName)
{
  return dataModel(modelName).species(name, compartmentName);
}

// static 
std::vector< cpsapiSpecies > cpsapi::getSpecies(const std::string & modelName)
{
  return dataModel(modelName).getSpecies();
}

// static
bool cpsapi::addGlobalQuantity(const std::string & name, const std::string & modelName)
{
  return dataModel(modelName).addGlobalQuantity(name);
}

// static
bool cpsapi::deleteGlobalQuantity(const std::string & name, const std::string & modelName)
{
  return dataModel(modelName).deleteGlobalQuantity(name);
}

// static 
cpsapiGlobalQuantity cpsapi::globalQuantity(const std::string & name, const std::string & modelName)
{
  return dataModel(modelName).globalQuantity(name);
}

// static 
std::vector< cpsapiGlobalQuantity > cpsapi::getGlobalQuantities(const std::string & modelName)
{
  return dataModel(modelName).getGlobalQuantities();
}

// static 
void cpsapi::init()
{
  CRootContainer::init(0, nullptr);
}

// static 
void cpsapi::release()
{
  CRootContainer::destroy();
}
