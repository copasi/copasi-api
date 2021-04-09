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

#define COPASI_MAIN

#include <copasi/core/CRootContainer.h>
#include <copasi/CopasiDataModel/CDataModel.h>
#include <copasi/core/CDataVector.h>

#include "cpsapi/core/cpsapiRoot.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"

CPSAPI_NAMESPACE_BEGIN
CPSAPI_NAMESPACE_USE

// static 
std::map< std::string, CDataModel * > cpsapiRoot::DataModels;

// static 
CDataModel * cpsapiRoot::pDefaultDataModel = nullptr;

// static 
CModel * cpsapiRoot::pDefaultModel = nullptr;

// static 
CCopasiTask * cpsapiRoot::pDefaultTask = nullptr;

// static 
CReportDefinition * cpsapiRoot::pDefaultReportDefinition = nullptr;

// static 
CPlotSpecification * cpsapiRoot::pDefaultPlotSpecification = nullptr;

// static 
CFunction * cpsapiRoot::pDefaultFunction = nullptr;

// static 
CUnitDefinition * cpsapiRoot::pDefaultUnitDefinition = nullptr;

// static 
cpsapiModel cpsapiRoot::addModel(const std::string & name)
{
  if (DataModels.find(name) != DataModels.end())
    return nullptr;

  changeDataModel(CRootContainer::addDatamodel());
  DataModels.insert(std::make_pair(name, pDefaultDataModel));

  return pDefaultDataModel->getModel();
}

// static
bool cpsapiRoot::deleteModel(const std::string & name)
{
  std::map< std::string, CDataModel * >::iterator found = DataModels.find(name);

  if (found == DataModels.end())
    return false;

  delete found->second;
  DataModels.erase(found);

  if (pDefaultDataModel == found->second)
    changeDataModel(nullptr);

  return true;
}

// static
CDataModel * cpsapiRoot::dataModel(const std::string & name)
{
  std::map< std::string, CDataModel * >::iterator found = DataModels.find(name);

  if (found != DataModels.end())
    return found->second;

  if (!name.empty())
    return nullptr;

  assertDefaultDataModel();

  return pDefaultDataModel;
}

// static
const std::set< std::string > cpsapiRoot::listModels()
{
  std::set< std::string > Set;
  std::map< std::string, CDataModel * >::const_iterator it = DataModels.begin();
  std::map< std::string, CDataModel * >::const_iterator end = DataModels.end();

  for (; it != end; ++it)
    Set.insert(it->first);

  return Set;
}

// static
bool cpsapiRoot::loadModel(const std::string & src)
{
  assertDefaultDataModel();

  return pDefaultDataModel->loadModel(src, nullptr);
}

// static
void cpsapiRoot::assertDefaultDataModel()
{
  if (pDefaultDataModel != nullptr)
    return;
    
  if (CRootContainer::getDatamodelList()->empty())
    addModel("default model");
  else
    changeDataModel(&CRootContainer::getDatamodelList()->operator[](0));
}

// static
void cpsapiRoot::changeDataModel(CDataModel * pDataModel)
{
  pDefaultDataModel = pDataModel;

  pDefaultModel = nullptr;
  pDefaultTask = nullptr;
  pDefaultReportDefinition = nullptr;
  pDefaultPlotSpecification = nullptr;
}

// static
cpsapiModel cpsapiRoot::model(const std::string & name)
{
  if (name.empty()
      && pDefaultModel != nullptr)
    return pDefaultModel;

  CDataModel * pDataModel = dataModel(name);

  if (pDataModel == nullptr)
    return nullptr;

  return pDataModel->getModel();
}

// static
void cpsapiRoot::beginTransaction(const std::string & name)
{
  cpsapiModel Model(model(name));

  if (Model)
    Model.beginTransaction();
}

// static
void cpsapiRoot::endTransaction(const std::string & name)

{
  cpsapiModel Model(model(name));

  if (Model)
    Model.endTransaction();
}

// static
bool cpsapiRoot::addCompartment(const std::string & name, const std::string & modelName)
{
  cpsapiModel Model(model(modelName));

  if (Model)
    return Model.addCompartment(name);

  return false;

}

// static
bool cpsapiRoot::deleteCompartment(const std::string & name, const std::string & modelName)
{
  cpsapiModel Model(model(modelName));

  if (Model)
    return Model.deleteCompartment(name);

  return false;
}

// static 
cpsapiCompartment cpsapiRoot::compartment(const std::string & name, const std::string & modelName)
{
  cpsapiModel Model(model(modelName));

  if (Model)
    return Model.compartment(name);

  return nullptr;
}

// static 
std::vector< cpsapiCompartment > cpsapiRoot::getCompartments(const std::string & modelName)
{
  cpsapiModel Model(model(modelName));

  if (Model)
    return Model.getCompartments();

  return std::vector< cpsapiCompartment >();
}

// static 
void changeCompartment(CCompartment * pCompartment);

// static 
void assertDefaultCompartment();

// static 
void cpsapiRoot::init()
{
  CRootContainer::init(0, nullptr);

  if (!CRootContainer::getDatamodelList()->empty())
    changeDataModel(&CRootContainer::getDatamodelList()->operator[](0));
}

// static 
void cpsapiRoot::release()
{
  CRootContainer::destroy();
}

CPSAPI_NAMESPACE_END