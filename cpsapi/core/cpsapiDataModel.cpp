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

#include <copasi/CopasiDataModel/CDataModel.h>
#include <copasi/core/CDataVector.h>

#include "cpsapi/core/cpsapiDataModel.h"
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiGlobalQuantity.h"

CPSAPI_NAMESPACE_USE

cpsapiDataModel::cpsapiDataModel(wrapped * pWrapped)
  : base(pWrapped, Type::DataModel)
{
  assertData(Data(*std::static_pointer_cast< base::Data >(mpData)));
}

cpsapiDataModel::cpsapiDataModel(const cpsapiDataModel & src)
  : base(src)
{}

// virtual 
cpsapiDataModel::~cpsapiDataModel()
{}

// virtual 
void cpsapiDataModel::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::DataModel);
  base::accept(visitor);
}

bool cpsapiDataModel::loadFromFile(const std::string & fileName)
{
  bool success = false;

  if (operator bool())
    {
      success = WRAPPED->loadFromFile(fileName, nullptr, false);

      if (success)
        {
          // We need to mark all old data as deleted before we actually delete it
          std::shared_ptr< Data > pData = DATA; 
          deleted(pData->mModel.getObject());
          pData->mpDefaultTask = nullptr;
          pData->mpDefaultReportDefinition = nullptr;
          pData->mpDefaultPlotSpecification = nullptr;
          WRAPPED->deleteOldData();

          pData->mModel = cpsapiModel(WRAPPED->getModel());
        }
    }

  return success;
}

bool cpsapiDataModel::loadFromString(const std::string & content, const std::string & referenceDir)
{
  bool success = false;

  if (operator bool())
    {
      success = WRAPPED->loadFromString(content, referenceDir, nullptr, false);

      if (success)
        {
          // We need to mark all old data as deleted before we actually delete it
          std::shared_ptr< Data > pData = DATA; 
          deleted(pData->mModel.getObject());
          pData->mpDefaultTask = nullptr;
          pData->mpDefaultReportDefinition = nullptr;
          pData->mpDefaultPlotSpecification = nullptr;
          WRAPPED->deleteOldData();

          pData->mModel = cpsapiModel(WRAPPED->getModel());
        }
    }

  return success;
}

cpsapiModel & cpsapiDataModel::model()
{
  if (operator bool()
      && !DATA->mModel)
    DATA->mModel = WRAPPED->getModel();

  return DATA->mModel;
}

void cpsapiDataModel::beginTransaction()
{
  model().beginTransaction();
}

void cpsapiDataModel::endTransaction()
{
  model().endTransaction();
}

bool cpsapiDataModel::addCompartment(const std::string & name)
{
  return model().addCompartment(name);
}

bool cpsapiDataModel::deleteCompartment(const std::string & name)
{
  return model().deleteCompartment(name);
}

cpsapiCompartment cpsapiDataModel::compartment(const std::string & name)
{
  return model().compartment(name);
}

cpsapiVector< cpsapiCompartment > cpsapiDataModel::getCompartments()
{
  return model().getCompartments();
}

bool cpsapiDataModel::addSpecies(const std::string & name, const std::string & compartmentName)
{
  return model().addSpecies(name, compartmentName);
}

bool cpsapiDataModel::deleteSpecies(const std::string & name, const std::string & compartmentName)
{
  return model().deleteSpecies(name, compartmentName);
}

cpsapiSpecies cpsapiDataModel::species(const std::string & name, const std::string & compartmentName)
{
  return model().species(name, compartmentName);
}

cpsapiVector< cpsapiSpecies > cpsapiDataModel::getSpecies()
{
  return model().getSpecies();
}

bool cpsapiDataModel::addGlobalQuantity(const std::string & name)
{
  return model().addGlobalQuantity(name);
}

bool cpsapiDataModel::deleteGlobalQuantity(const std::string & name)
{
  return model().deleteGlobalQuantity(name);
}

cpsapiGlobalQuantity cpsapiDataModel::globalQuantity(const std::string & name)
{
  return model().globalQuantity(name);
}

cpsapiVector< cpsapiGlobalQuantity > cpsapiDataModel::getGlobalQuantities()
{
  return model().getGlobalQuantities();
}

