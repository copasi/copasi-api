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

CPSAPI_NAMESPACE_USE

// static 
std::map< std::string, CDataModel * > cpsapi::DataModels;

// static 
CDataModel * cpsapi::pDefaultDataModel = nullptr;

// static 
CFunction * cpsapi::pDefaultFunction = nullptr;

// static 
CUnitDefinition * cpsapi::pDefaultUnitDefinition = nullptr;

// static 
cpsapiDataModel cpsapi::addDataModel(const std::string & name)
{
  if (DataModels.find(name) != DataModels.end())
    return nullptr;

  pDefaultDataModel = CRootContainer::addDatamodel();
  DataModels.insert(std::make_pair(name, pDefaultDataModel));

  // We start with the names being the same
  model().getObject()->setObjectName(name);
  
  return pDefaultDataModel;
}

// static
bool cpsapi::deleteDataModel(const std::string & name)
{
  std::map< std::string, CDataModel * >::iterator found = DataModels.find(name);

  if (found == DataModels.end())
    return false;

  if (pDefaultDataModel == found->second)
    pDefaultDataModel = nullptr;

  delete found->second;
  DataModels.erase(found);

  return true;
}

// static
cpsapiDataModel cpsapi::dataModel(const std::string & name)
{
  if (name.empty())
    {
      if (pDefaultDataModel == nullptr
          && CRootContainer::getDatamodelList()->empty())
        return addDataModel("default model");

      return pDefaultDataModel;
    }

  std::map< std::string, CDataModel * >::iterator found = DataModels.find(name);

  if (found != DataModels.end())
    return found->second;

  return nullptr;
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

  std::for_each(DataModels.begin(), DataModels.end(), [&Set](const std::pair< std::string, CDataModel * > & mapIterator){
    Set.insert(mapIterator.first);
  });

  return Set;
}

// static
bool cpsapi::load(const std::string & src, const std::string & modelName)
{
  return dataModel(modelName).load(src);
}

// static
cpsapiModel cpsapi::model(const std::string & name)
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
void cpsapi::init()
{
  CRootContainer::init(0, nullptr);
}

// static 
void cpsapi::release()
{
  CRootContainer::destroy();
}
